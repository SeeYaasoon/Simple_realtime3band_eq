/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "CustomLookAndFeel.h"

//==============================================================================
class SimpleRealtimeEQAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    SimpleRealtimeEQAudioProcessorEditor (SimpleRealtimeEQAudioProcessor&);
    ~SimpleRealtimeEQAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    SimpleRealtimeEQAudioProcessor& audioProcessor;

    CustomLookAndFeel customLookAndFeel;

    // EQ sliders
    juce::Slider lowFreqSlider;
    juce::Slider lowGainSlider;
    juce::Slider lowSSlider;

    juce::Slider midFreqSlider;
    juce::Slider midGainSlider;
    juce::Slider midQSlider;

    juce::Slider highFreqSlider;
    juce::Slider highGainSlider;
    juce::Slider highSSlider;

    // Saturation sliders
    juce::Slider satDriveSlider;
    juce::Slider satBiasSlider;
    juce::Slider satMixSlider;

    // EQ labels
    juce::Label lowFreqLabel;
    juce::Label lowGainLabel;
    juce::Label lowSLabel;

    juce::Label midFreqLabel;
    juce::Label midGainLabel;
    juce::Label midQLabel;

    juce::Label highFreqLabel;
    juce::Label highGainLabel;
    juce::Label highSLabel;

    // Saturation labels
    juce::Label satDriveLabel;
    juce::Label satBiasLabel;
    juce::Label satMixLabel;

    // EQ attachments
    std::unique_ptr<SliderAttachment> lowFreqAttachment;
    std::unique_ptr<SliderAttachment> lowGainAttachment;
    std::unique_ptr<SliderAttachment> lowSAttachment;

    std::unique_ptr<SliderAttachment> midFreqAttachment;
    std::unique_ptr<SliderAttachment> midGainAttachment;
    std::unique_ptr<SliderAttachment> midQAttachment;

    std::unique_ptr<SliderAttachment> highFreqAttachment;
    std::unique_ptr<SliderAttachment> highGainAttachment;
    std::unique_ptr<SliderAttachment> highSAttachment;

    // Saturation attachments
    std::unique_ptr<SliderAttachment> satDriveAttachment;
    std::unique_ptr<SliderAttachment> satBiasAttachment;
    std::unique_ptr<SliderAttachment> satMixAttachment;

    void setupSlider (juce::Slider& slider);
    void setupLabel (juce::Label& label, const juce::String& text);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimpleRealtimeEQAudioProcessorEditor)
};
