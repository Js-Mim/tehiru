#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "compute_parameters.h"


class TehiruAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    TehiruAudioProcessor();
    ~TehiruAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
	void processBlock(AudioSampleBuffer&, MidiBuffer&) override;
	void reset() override;
    
	bool hasEditor() const override					{ return true; }
    AudioProcessorEditor* createEditor() override;
    
    //==============================================================================
	const String getName() const override            { return JucePlugin_Name; }
	
	const String getInputChannelName(int channelIndex) const override;
	const String getOutputChannelName(int channelIndex) const override;
	bool isInputChannelStereoPair(int index) const override;
	bool isOutputChannelStereoPair(int index) const override;

	bool acceptsMidi() const override;
	bool producesMidi() const override;
	bool silenceInProducesSilenceOut() const override;
	double getTailLengthSeconds() const override;

	int getNumPrograms() override                                              { return 1; }
	int getCurrentProgram() override                                           { return 0; }
	void setCurrentProgram(int) override                             {}
	const String getProgramName(int) override                        { return "Default"; }
	void changeProgramName(int, const String&) override  {}

	void getStateInformation(MemoryBlock& destData) override;
	void setStateInformation(const void* data, int sizeInBytes) override;

    //==============================================================================


	// UI Parameters
	int lastUIWidth_, lastUIHeight_;

	// Adjustable parameters:
	AudioProcessorParameter* RecordingTechnique_;	// Recording technique caller
	AudioProcessorParameter* SourceAngle_;			// Angle for source positioning {-77 to +77 degrees}
	AudioProcessorParameter* focus_;				// Focus coefficient {0.05 - 0.80}
	float fparam_ = 0.05;
	int effparam_ = 1;
	int lparam_ = 0;

private:
	// Circular buffer variables for the delay line
	AudioSampleBuffer delayBuffer_;
	int delayBufferLength_;
	int delayReadPositionL_, delayReadPositionR_, delayWritePosition_;
	int delayLength_;

	// Processing parameters
	int operation_;
	float VMic1_, VMic2_, VM1HsF_, VM2HsF_, a1L_, a2L_, b0L_, b1L_, b2L_,
		a1R_, a2R_, b0R_, b1R_, b2R_, xrL_[2], xrR_[2], yrL_[2], yrR_[2],
		a1LL_, a2LL_, b0LL_, b1LL_, b2LL_, a1LR_, a2LR_, b0LR_, b1LR_, b2LR_,
		xrL2_[2], xrR2_[2], yrL2_[2], yrR2_[2], b0A_, b1A_, b2A_, a1A_, a2A_,
		b0B_, b1B_, b2B_, a1B_, a2B_, xRA_[2], yRA_[2], xRB_[2], yRB_[2],
		pG1_, pG2_;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TehiruAudioProcessor)
};


#endif  // PLUGINPROCESSOR_H_INCLUDED
