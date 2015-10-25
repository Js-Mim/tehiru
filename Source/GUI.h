#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

class TehiruAudioProcessorEditor : public AudioProcessorEditor,
								   public SliderListener,
								   public Timer

{
public:
	TehiruAudioProcessorEditor(TehiruAudioProcessor&);
	~TehiruAudioProcessorEditor();
	//==============================================================================
	void paint(Graphics&) override;
	void resized() override;
	void timerCallback() override;
	void computeTransformations(Slider *);
	void computeMappingValues(Slider *, float);
	void sliderValueChanged(Slider*) override;
	void sliderDragStarted(Slider*) override;
	void sliderDragEnded(Slider*) override;

    // Binary resources:
    static const char* backgn_png;
    static const int backgn_pngSize;
    static const char* state1_png;
    static const int state1_pngSize;
    static const char* state2_png;
    static const int state2_pngSize;
    static const char* state3_png;
    static const int state3_pngSize;
    static const char* state4_png;
    static const int state4_pngSize;
    static const char* state5_png;
    static const int state5_pngSize;
    static const char* state6_png;
    static const int state6_pngSize;
    static const char* state7_png;
    static const int state7_pngSize;
    static const char* blackknob_png;
    static const int blackknob_pngSize;


private:

	TehiruAudioProcessor& processor;
	ScopedPointer<ResizableCornerComponent> resizer_;
	ComponentBoundsConstrainer resizeLimits_;

	// Images
	Image cachedImage_backgn_png, cachedImage_blackknob_png, cachedImage_state1_png,
		cachedImage_state2_png, cachedImage_state3_png, cachedImage_state4_png, cachedImage_state5_png, 
		cachedImage_state6_png, cachedImage_state7_png;
	
	// Image properties
	int blackknobHeight_, blackknobWidth_;

	// Image transformation
	AffineTransform focusTransform_, locusTransform_;
	float locusRotation_ = 6.23f;
	float focusRotation_ = 3.6652f;
	float buttonScale_ = 0.623f;
	int initBX_ = 620;
	int initBY_ = 290;
	int initWX_ = 45;
	int initWY_ = 290;

	// Slider Labels
	//Label RecTechnLabel_, SourceAngleLabel_, FocusLabel_;

	// Rotary Sliders
	Slider RecTechnSlider_, SourceAngleSlider_, FocusSlider_;

	// Alpha States
	float alpha1_ = 0.890f;
	float alpha2_, alpha3_, alpha4_, alpha5_, alpha6_, alpha7_ = 0.0f;

	// Automation & Graphics control variables
	int get_eff_param, get_loc_param;
	float get_foc_param, newRotation_;
	float atg_eff_param = 1.0f;
	int ats_eff_param = 1;
	int atg_loc_param, ats_loc_param = 0;

	TehiruAudioProcessor& getProcessor() const
	{
		return static_cast<TehiruAudioProcessor&> (processor);
	}

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TehiruAudioProcessorEditor)
};


#endif
