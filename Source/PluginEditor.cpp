/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//================================================== ============================
Gain_pitchshiftAudioProcessorEditor::Gain_pitchshiftAudioProcessorEditor (Gain_pitchshiftAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    //gain slider
    GainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    GainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20);
    GainSlider.setRange(0.0f, 1.0f, 0.01f);
    GainSlider.setValue(0.5f);
    GainSlider.addListener(this);
    addAndMakeVisible(GainSlider);
    
    //pitch slider
    PitchSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    PitchSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20);
    PitchSlider.setRange(0.0f, 1.0f, 0.01f);
    PitchSlider.setValue(0.5f);
    PitchSlider.addListener(this);
    addAndMakeVisible(PitchSlider);
    
    
    setSize (400, 300);
}

Gain_pitchshiftAudioProcessorEditor::~Gain_pitchshiftAudioProcessorEditor()
{
}

//==============================================================================
void Gain_pitchshiftAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colours::black);
}

void Gain_pitchshiftAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    GainSlider.setBounds(100, getHeight() / 2 - 75, 100, 150);
    
    PitchSlider.setBounds(getWidth() - 100, getHeight() / 2 - 75, 100, 150);
}

void Gain_pitchshiftAudioProcessorEditor::sliderValueChanged (juce::Slider *slider){
    
    if (slider == &GainSlider){
        audioProcessor.Gain = GainSlider.getValue();
    }
    
    if (slider == &PitchSlider){
        audioProcessor.Pitch = PitchSlider.getValue();
    }
}
