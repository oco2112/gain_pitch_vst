/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class Gain_pitchshiftAudioProcessorEditor  : public juce::AudioProcessorEditor,
public juce::Slider::Listener

{
public:
    Gain_pitchshiftAudioProcessorEditor (Gain_pitchshiftAudioProcessor&);
    ~Gain_pitchshiftAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void sliderValueChanged (juce::Slider* slider) override;

private:
    juce::Slider GainSlider;
    juce::Slider PitchSlider;
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Gain_pitchshiftAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Gain_pitchshiftAudioProcessorEditor)
};
