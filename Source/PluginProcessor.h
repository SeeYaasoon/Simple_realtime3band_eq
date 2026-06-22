/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Biquad.h"
#include "EQParameters.h"

//==============================================================================
/**
*/
class SimpleRealtimeEQAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    SimpleRealtimeEQAudioProcessor();
    ~SimpleRealtimeEQAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState apvts;

    private:
        //==============================================================================
        Biquad lowShelfL;
        Biquad midPeakL;
        Biquad highShelfL;

        Biquad lowShelfR;
        Biquad midPeakR;
        Biquad highShelfR;
        float dcPrevXL = 0.0f;
        float dcPrevYL = 0.0f;

        float dcPrevXR = 0.0f;
        float dcPrevYR = 0.0f;
        float inputRMS = 0.0f;

        double currentSampleRate = 44100.0;

        void updateFilters();

        static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimpleRealtimeEQAudioProcessor)
    };
