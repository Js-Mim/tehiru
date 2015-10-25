#include "PluginProcessor.h"
#include "GUI.h"
using namespace std;

AudioProcessor* JUCE_CALLTYPE createPluginFilter();

class FloatParameter : public AudioProcessorParameter
{
public:

	FloatParameter(float defaultParameterValue, const String& paramName)
	  : defaultValue(defaultParameterValue),
		value(defaultParameterValue),
		name(paramName)
	{

	}

	float getValue() const override
	{
		return value;
	}

	void setValue(float newValue) override
	{
		value = newValue;
	}

	float getDefaultValue() const override
	{
		return defaultValue;
	}

	String getName(int) const override
	{
		return name;
	}

	String getLabel() const override
	{
		return String();
	}

	float getValueForText(const String& text) const override
	{
		return text.getFloatValue();
	}

private:
	float defaultValue, value;
	String name;
};

// Main Plugin Operations

TehiruAudioProcessor::TehiruAudioProcessor() :
						// Initialize delay buffer
						delayBuffer_(2, 1)

// Initialize rest of parameters/variables
{
	// Set default values:
	addParameter(RecordingTechnique_ = new FloatParameter(1.0f, "Effectum"));
	addParameter(SourceAngle_ = new FloatParameter(0.0f, "Locus"));
	addParameter(focus_ = new FloatParameter(0.05f, "Focus"));

	operation_ = 0;
	VMic2_ = VMic1_ = VM1HsF_ = VM2HsF_ = 1.0f;
	delayLength_ = 0;
	pG1_ = 1.0f;
	pG2_ = 1.0f;

	// Filter registers
	// High-Shelving
	xrL_[0] = xrL_[1] = yrL_[0] = yrL_[1] = xrR_[0] = xrR_[1] = yrR_[0] = yrR_[1] = 0.0;
	// Low-Shelving
	xrL2_[0] = xrR2_[0] = yrL2_[0] = yrR2_[0] = xrL2_[1] = xrR2_[1] = yrL2_[1] = yrR2_[1] = 0.0;
	// Alll-pass
	xRA_[0] = xRA_[1] = yRA_[0] = yRA_[1] = xRB_[0] = xRB_[1] = yRB_[0] = yRB_[1] = 0.0;

	// Filtering coefficients
	// High shelving
	a1L_ = a2L_ = b0L_ = b1L_ = b2L_ = 1.0;
	a1R_ = a2R_ = b0R_ = b1R_ = b2R_ = 1.0;
	// Low Shelving
	a1LL_ = a2LL_ = b0LL_ = b1LL_ = b2LL_ = 1.0;
	a1LR_ = a2LR_ = b0LR_ = b1LR_ = b2LR_ = 1.0;

	// Initialise circular buffer
	delayReadPositionL_ = 0;
	delayReadPositionR_ = 0;
	delayWritePosition_ = 0;
	
	//UI
	lastUIWidth_ = 781;
	lastUIHeight_ = 450;
}

TehiruAudioProcessor::~TehiruAudioProcessor()
{
}

void TehiruAudioProcessor::prepareToPlay (double sampleRate, int)
{
	// Initialise delay buffer
	delayBufferLength_ = (int)(sampleRate);
	if (delayBufferLength_ < 1)
		delayBufferLength_ = 1;

	delayBuffer_.setSize(2, delayBufferLength_);
	delayBuffer_.clear();

	delayReadPositionL_ = (int)(delayWritePosition_ - delayLength_ + delayBufferLength_) % delayBufferLength_;
	delayReadPositionR_ = (int)(delayWritePosition_ - delayLength_ + delayBufferLength_) % delayBufferLength_;

	// Grab pre-computed allpass coefficients
	tie(b0A_, b1A_, b2A_, a1A_, a2A_, b0B_, b1B_, b2B_, a1B_, a2B_) = getPrecompAllpasCoeffs();

	// M/S Panorama Gain Control
	pG1_ = 1.0f;
	pG2_ = 1.0f;
}

void TehiruAudioProcessor::releaseResources()
{
	reset();
}

void TehiruAudioProcessor::reset()
{
	delayBuffer_.clear();
}

void TehiruAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer&)
{
	// Buffer Properties
	const int numInputChannels = getNumInputChannels();     // Input Channels
	const int numOutputChannels = getNumOutputChannels();   // Output Channels
	const int numSamples = buffer.getNumSamples();          // Number of samples inside current buffer
	const int fs_ = (int) round(getSampleRate());			// Sampling Rate

	// Computation of IID,ITD
	tie(delayLength_, VMic1_, VMic2_, operation_, VM1HsF_, VM2HsF_) =
		compute_params(effparam_, lparam_, fparam_, fs_);

	if (operation_ == 0)
	{
		if (lparam_ < 0)
		{
			delayReadPositionR_ = (int)(delayWritePosition_ - delayLength_ + delayBufferLength_) % delayBufferLength_;
			delayReadPositionL_ = (int)(delayWritePosition_ + delayBufferLength_) % delayBufferLength_;
		}
		else if (lparam_ > 0)
		{
			delayReadPositionL_ = (int)(delayWritePosition_ - delayLength_ + delayBufferLength_) % delayBufferLength_;
			delayReadPositionR_ = (int)(delayWritePosition_ + delayBufferLength_) % delayBufferLength_;
		}
		else
		{
			delayReadPositionL_ = (int)(delayWritePosition_ - delayLength_ + delayBufferLength_) % delayBufferLength_;
			delayReadPositionR_ = (int)(delayWritePosition_ - delayLength_ + delayBufferLength_) % delayBufferLength_;
		}
	}

	// Swap delay-lines for M/S processing
	else
	{
		delayReadPositionL_ = (int)(delayWritePosition_ + delayBufferLength_) % delayBufferLength_;
		delayReadPositionR_ = (int)(delayWritePosition_ - delayLength_ + delayBufferLength_) % delayBufferLength_;

	}

	// Compute filter coefficients
	tie(b0L_, b1L_, b2L_, a1L_, a2L_) = getHighShelvCoeffs(VM1HsF_, 8110, fs_);
	tie(b0R_, b1R_, b2R_, a1R_, a2R_) = getHighShelvCoeffs(VM2HsF_, 8110, fs_);
	tie(b0LL_, b1LL_, b2LL_, a1LL_, a2LL_) = getLowShelvCoeffs(VM1HsF_, 88, fs_);
	tie(b0LR_, b1LR_, b2LR_, a1LR_, a2LR_) = getLowShelvCoeffs(VM2HsF_, 88, fs_);

	// Compute M/S panorama gain control
	pG2_ = 1.0f;
	float const pG1 = -(tand(lparam_) - tand(80.0f)) / (tand(lparam_) + tand(80.0f) + eps);
	pG1_ = pG1 / sqrt(pG1*pG1 + pG2_*pG2_);
	pG2_ = pG2_ / sqrt(pG1*pG1 + pG2_*pG2_);

	// Check for stereo(mono summation) or mono(clear the second) input
	if (numInputChannels > 1)
	{
		float *dL = buffer.getSampleData(0);
		float *dR = buffer.getSampleData(1);
		for (int i = 0; i < numSamples; ++i)
		{
			dL[i] = (dL[i] + dR[i])*0.5f;
		}

		buffer.copyFrom(0, 0, dL, numSamples);
		buffer.copyFrom(1, 0, dL, numSamples);
	}
	else
	{
		buffer.clear(0, 0, numSamples);
	}

	// Acquire Buffers
	float *channelDataL = buffer.getSampleData(0);
	float *channelDataR = buffer.getSampleData(1);

	// Acquire Delay circular buffers
	float *delayDataL = delayBuffer_.getSampleData(0);
	float *delayDataR = delayBuffer_.getSampleData(1);

	if (operation_ == 0)
	{
		for (int i = 0; i < numSamples; ++i)
		{
			// Acquire buffer samples
			float const inL = channelDataL[i];
			float const inR = channelDataR[i];
			float outL, outR, inLnew, inRnew, inLnew2, inRnew2;

			// Filter the input samples
			// Left channel
			// High-Shelving
			inLnew = b0L_ * inL + b1L_ * xrL_[0] + b2L_ * xrL_[1] - a1L_ * yrL_[0] - a2L_ * yrL_[1];
			xrL_[1] = xrL_[0];
			xrL_[0] = inL;
			yrL_[1] = yrL_[0];
			yrL_[0] = inLnew;

			// Low-Shelving
			inLnew2 = b0LL_ * inLnew + b1LL_ * xrL2_[0] + b2LL_ * xrL2_[1] - a1LL_ * yrL2_[0] - a2LL_ * yrL2_[1];
			xrL2_[1] = xrL2_[0];
			xrL2_[0] = inLnew;
			yrL2_[1] = yrL2_[0];
			yrL2_[0] = inLnew2;

			// Right channel
			// High-Shelving
			inRnew = b0R_ * inR + b1R_ * xrR_[0] + b2R_ * xrR_[1] - a1R_ * yrR_[0] - a2R_ * yrR_[1];
			xrR_[1] = xrR_[0];
			xrR_[0] = inR;
			yrR_[1] = yrR_[0];
			yrR_[0] = inRnew;

			// Low-Shelving
			inRnew2 = b0LR_ * inRnew + b1LR_ * xrR2_[0] + b2LR_ * xrR2_[1] - a1LR_ * yrR2_[0] - a2LR_ * yrR2_[1];
			xrR2_[1] = xrR2_[0];
			xrR2_[0] = inRnew;
			yrR2_[1] = yrR2_[0];
			yrR2_[0] = inRnew2;

			// Write into the delay buffer the new filtered samples
			delayDataL[delayWritePosition_] = inLnew2 * VMic1_;
			delayDataR[delayWritePosition_] = inRnew2 * VMic2_;

			// Read from delay buffer
			outL = delayDataL[delayReadPositionL_];
			outR = delayDataR[delayReadPositionR_];

			// Update read/write positions and check for overrun
			if (++delayReadPositionL_ >= delayBufferLength_)
				delayReadPositionL_ = 0;
			if (++delayReadPositionR_ >= delayBufferLength_)
				delayReadPositionR_ = 0;
			if (++delayWritePosition_ >= delayBufferLength_)
				delayWritePosition_ = 0;

			// Store the output samples, stereo output
			channelDataL[i] = outL;
			channelDataR[i] = outR;
		}
	}
	// M/S processing
	else if (operation_ == 1)
	{
		for (int i = 0; i < numSamples; ++i)
		{
			const float inL = channelDataL[i];
			const float inR = channelDataR[i];
			float outL, outR, inLnew, inRnew, inLnew2, inRnew2, phs_shft1, phs_shft2;

			// Filter the input samples
			// Left channel
			// High-Shelving
			inLnew = b0L_ * inL + b1L_ * xrL_[0] + b2L_ * xrL_[1] - a1L_ * yrL_[0] - a2L_ * yrL_[1];
			xrL_[1] = xrL_[0];
			xrL_[0] = inL;
			yrL_[1] = yrL_[0];
			yrL_[0] = inLnew;

			// Low-Shelving
			inLnew2 = b0LL_ * inLnew + b1LL_ * xrL2_[0] + b2LL_ * xrL2_[1] - a1LL_ * yrL2_[0] - a2LL_ * yrL2_[1];
			xrL2_[1] = xrL2_[0];
			xrL2_[0] = inLnew;
			yrL2_[1] = yrL2_[0];
			yrL2_[0] = inLnew2;

			// Right channel
			// High-Shelving
			inRnew = b0R_ * inR + b1R_ * xrR_[0] + b2R_ * xrR_[1] - a1R_ * yrR_[0] - a2R_ * yrR_[1];
			xrR_[1] = xrR_[0];
			xrR_[0] = inR;
			yrR_[1] = yrR_[0];
			yrR_[0] = inRnew;

			// Low-Shelving
			inRnew2 = b0LR_ * inRnew + b1LR_ * xrR2_[0] + b2LR_ * xrR2_[1] - a1LR_ * yrR2_[0] - a2LR_ * yrR2_[1];
			xrR2_[1] = xrR2_[0];
			xrR2_[0] = inRnew;
			yrR2_[1] = yrR2_[0];
			yrR2_[0] = inRnew2;


			// Apply all-pass filter to second(R) channel
			phs_shft1 = b0A_ * inRnew2 + b1A_ * xRA_[0] + b2A_ * xRA_[1] - a1A_ * yRA_[0] - a2A_ * yRA_[1];
			xRA_[1] = xRA_[0];
			xRA_[0] = inRnew2;
			yRA_[1] = yRA_[0];
			yRA_[0] = phs_shft1;

			phs_shft2 = b0B_ * inRnew2 + b1B_ * xRB_[0] + b2B_ * xRB_[1] - a1B_ * yRB_[0] - a2B_ * yRB_[1];
			xRB_[1] = xRB_[0];
			xRB_[0] = inRnew2;
			yRB_[1] = yRB_[0];
			yRB_[0] = phs_shft2;

			// Compute difference signal
			inRnew2 = (inR - phs_shft1) + (inR - phs_shft2);
			inRnew2 = inRnew2 * 0.25f;

			// Write first, into the delay buffer the new filtered samples
			delayDataL[delayWritePosition_] = inLnew2;
			delayDataR[delayWritePosition_] = inRnew2;

			// Process the read samples
			if (lparam_ != 0)
			{
				outL = ((VMic2_ *(0.95f - fparam_)) * delayDataR[delayReadPositionR_]) +
					(delayDataL[delayReadPositionL_] * (VMic1_ * pG1_));

				outR = ((-VMic2_ *(0.95f - fparam_)) * delayDataR[delayReadPositionR_]) +
					(delayDataL[delayReadPositionL_] * (VMic1_ * pG2_));

			}
			else
			{
				outL = (((1.05f - fparam_)) * delayDataR[delayReadPositionR_])
					+ (delayDataL[delayReadPositionL_] * VMic1_);

				outR = (-((1.05f - fparam_)) * delayDataR[delayReadPositionR_])
					+ (delayDataL[delayReadPositionL_] * VMic1_);
			}

			// Update read/write positions and check for overrun
			if (++delayReadPositionL_ >= delayBufferLength_)
				delayReadPositionL_ = 0;
			if (++delayReadPositionR_ >= delayBufferLength_)
				delayReadPositionR_ = 0;
			if (++delayWritePosition_ >= delayBufferLength_)
				delayWritePosition_ = 0;

			// Store the output samples. Stereo output
			channelDataL[i] = outL;
			channelDataR[i] = outR;
		}

	}

	// Clear if any unwanted information
	for (int i = 2; i < numOutputChannels; ++i)
	{
		buffer.clear(i, 0, buffer.getNumSamples());
	}
}


AudioProcessorEditor* TehiruAudioProcessor::createEditor()
{
	return new TehiruAudioProcessorEditor(*this);

}


void TehiruAudioProcessor::getStateInformation(MemoryBlock& destData)
{
	XmlElement xml("EmptySetPlugins");

	xml.setAttribute("uiWidth", lastUIWidth_);
	xml.setAttribute("uiHeight", lastUIHeight_);
	xml.setAttribute("Effectum", effparam_);
	xml.setAttribute("Locus", lparam_);
	xml.setAttribute("Focus", fparam_);

	copyXmlToBinary(xml, destData);
}

void TehiruAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	ScopedPointer<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

	if (xmlState != nullptr)
	{
		if (xmlState->hasTagName("EmptySetPlugins"))
		{
			lastUIWidth_ = xmlState->getIntAttribute("uiWidth", lastUIWidth_);
			lastUIHeight_ = xmlState->getIntAttribute("uiHeight", lastUIHeight_);

			RecordingTechnique_->setValue(xmlState->getIntAttribute("Effectum", effparam_));
			SourceAngle_->setValue(xmlState->getIntAttribute("Locus", lparam_));
			focus_->setValue((float)xmlState->getDoubleAttribute("Focus", fparam_));
		}
	}
}

const String TehiruAudioProcessor::getInputChannelName(int channelIndex) const
{
	return String(channelIndex + 1);
}

const String TehiruAudioProcessor::getOutputChannelName(int channelIndex) const
{
	return String(channelIndex + 1);
}

bool TehiruAudioProcessor::isInputChannelStereoPair(int) const
{
	return true;
}

bool TehiruAudioProcessor::isOutputChannelStereoPair(int) const
{
	return true;
}

bool TehiruAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
	return true;
#else
	return false;
#endif
}

bool TehiruAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
	return true;
#else
	return false;
#endif
}

bool TehiruAudioProcessor::silenceInProducesSilenceOut() const
{
	return false;
}

double TehiruAudioProcessor::getTailLengthSeconds() const
{
	return 0.0;
}

// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TehiruAudioProcessor();
}
