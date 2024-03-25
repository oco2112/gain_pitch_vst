/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Gain_pitchshiftAudioProcessor::Gain_pitchshiftAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

Gain_pitchshiftAudioProcessor::~Gain_pitchshiftAudioProcessor()
{
}

//==============================================================================
const juce::String Gain_pitchshiftAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Gain_pitchshiftAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Gain_pitchshiftAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Gain_pitchshiftAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Gain_pitchshiftAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Gain_pitchshiftAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Gain_pitchshiftAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Gain_pitchshiftAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String Gain_pitchshiftAudioProcessor::getProgramName (int index)
{
    return {};
}

void Gain_pitchshiftAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void Gain_pitchshiftAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void Gain_pitchshiftAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Gain_pitchshiftAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void Gain_pitchshiftAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            auto* channelData = buffer.getWritePointer (channel);
            
            for (int sample = 0; sample < buffer.getNumSamples(); ++sample){
                // Apply gain
                channelData[sample] *= Gain;

                // Apply pitch shifting
                float idx = static_cast<float>(sample);
                float modIdx = idx * ((Pitch * (1.5 - 0.5)) + 0.5);
                // Ensure modIdx stays within buffer bounds
                modIdx = std::fmod(modIdx, buffer.getNumSamples());
                // Interpolation
                int idx1 = static_cast<int>(modIdx);
                int idx2 = (idx1 + 1) % buffer.getNumSamples(); // Wrap around if needed
                float frac = modIdx - idx1;
                float interpolatedSample = (1 - frac) * channelData[idx1] + frac * channelData[idx2];
                channelData[sample] = interpolatedSample;
            }

            // ..do something to the data...
        }
    

              
}

//==============================================================================
bool Gain_pitchshiftAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Gain_pitchshiftAudioProcessor::createEditor()
{
    return new Gain_pitchshiftAudioProcessorEditor (*this);
}

//==============================================================================
void Gain_pitchshiftAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Gain_pitchshiftAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Gain_pitchshiftAudioProcessor();
}
