/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SimpleRealtimeEQAudioProcessorEditor::SimpleRealtimeEQAudioProcessorEditor (SimpleRealtimeEQAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (760, 420);

    setupSlider (lowFreqSlider);
    setupSlider (lowGainSlider);
    setupSlider (lowSSlider);

    setupSlider (midFreqSlider);
    setupSlider (midGainSlider);
    setupSlider (midQSlider);

    setupSlider (highFreqSlider);
    setupSlider (highGainSlider);
    setupSlider (highSSlider);
    setupSlider (satDriveSlider);
    setupSlider (satBiasSlider);
    setupSlider (satMixSlider);

    setupLabel (lowFreqLabel,  "Low Freq");
    setupLabel (lowGainLabel,  "Low Gain");
    setupLabel (lowSLabel,     "Low Slope");

    setupLabel (midFreqLabel,  "Mid Freq");
    setupLabel (midGainLabel,  "Mid Gain");
    setupLabel (midQLabel,     "Mid Q");

    setupLabel (highFreqLabel, "High Freq");
    setupLabel (highGainLabel, "High Gain");
    setupLabel (highSLabel,    "High Slope");
    setupLabel (satDriveLabel, "Drive");
    setupLabel (satBiasLabel,  "Bias");
    setupLabel (satMixLabel,   "Mix");

    lowFreqAttachment = std::make_unique<SliderAttachment>(
        audioProcessor.apvts,
        EQParameterIDs::lowFreq,
        lowFreqSlider);

    lowGainAttachment = std::make_unique<SliderAttachment>(
        audioProcessor.apvts,
        EQParameterIDs::lowGain,
        lowGainSlider);

    lowSAttachment = std::make_unique<SliderAttachment>(
        audioProcessor.apvts,
        EQParameterIDs::lowS,
        lowSSlider);

    midFreqAttachment = std::make_unique<SliderAttachment>(
        audioProcessor.apvts,
        EQParameterIDs::midFreq,
        midFreqSlider);

    midGainAttachment = std::make_unique<SliderAttachment>(
        audioProcessor.apvts,
        EQParameterIDs::midGain,
        midGainSlider);

    midQAttachment = std::make_unique<SliderAttachment>(
        audioProcessor.apvts,
        EQParameterIDs::midQ,
        midQSlider);

    highFreqAttachment = std::make_unique<SliderAttachment>(
        audioProcessor.apvts,
        EQParameterIDs::highFreq,
        highFreqSlider);

    highGainAttachment = std::make_unique<SliderAttachment>(
        audioProcessor.apvts,
        EQParameterIDs::highGain,
        highGainSlider);

    highSAttachment = std::make_unique<SliderAttachment>(
        audioProcessor.apvts,
        EQParameterIDs::highS,
        highSSlider);
    satDriveAttachment = std::make_unique<SliderAttachment>(
        audioProcessor.apvts,
        EQParameterIDs::satDrive,
        satDriveSlider);

    satBiasAttachment = std::make_unique<SliderAttachment>(
        audioProcessor.apvts,
        EQParameterIDs::satBias,
        satBiasSlider);

    satMixAttachment = std::make_unique<SliderAttachment>(
        audioProcessor.apvts,
        EQParameterIDs::satMix,
        satMixSlider);
}

SimpleRealtimeEQAudioProcessorEditor::~SimpleRealtimeEQAudioProcessorEditor()
{
  
        lowFreqSlider.setLookAndFeel (nullptr);
        lowGainSlider.setLookAndFeel (nullptr);
        lowSSlider.setLookAndFeel (nullptr);

        midFreqSlider.setLookAndFeel (nullptr);
        midGainSlider.setLookAndFeel (nullptr);
        midQSlider.setLookAndFeel (nullptr);

        highFreqSlider.setLookAndFeel (nullptr);
        highGainSlider.setLookAndFeel (nullptr);
        highSSlider.setLookAndFeel (nullptr);

        satDriveSlider.setLookAndFeel (nullptr);
        satBiasSlider.setLookAndFeel (nullptr);
        satMixSlider.setLookAndFeel (nullptr);
    }


//==============================================================================
void SimpleRealtimeEQAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour::fromRGB (14, 14, 18));

    auto area = getLocalBounds().toFloat();

    juce::ColourGradient gradient (juce::Colour::fromRGB (24, 24, 32),
                                   area.getCentreX(),
                                   0.0f,
                                   juce::Colour::fromRGB (10, 10, 14),
                                   area.getCentreX(),
                                   area.getBottom(),
                                   false);

    g.setGradientFill (gradient);
    g.fillRect (area);

    g.setColour (juce::Colours::white);
    g.setFont (juce::Font (24.0f, juce::Font::bold));

    g.drawText ("Simple Realtime 3-Band EQ",
                20,
                15,
                getWidth() - 40,
                35,
                juce::Justification::centred);

    // Панель сатурации
    g.setColour (juce::Colour::fromRGB (28, 28, 36));
    g.fillRoundedRectangle (40.0f, 70.0f, getWidth() - 80.0f, 150.0f, 12.0f);

    g.setColour (juce::Colour::fromRGB (255, 170, 60));
    g.setFont (juce::Font (16.0f, juce::Font::bold));

    g.drawText ("PREAMP SATURATION",
                40,
                80,
                getWidth() - 80,
                25,
                juce::Justification::centred);

    // Панели EQ
    g.setColour (juce::Colour::fromRGB (28, 28, 36));
    g.fillRoundedRectangle (40.0f, 250.0f, 220.0f, 230.0f, 12.0f);
    g.fillRoundedRectangle (300.0f, 250.0f, 220.0f, 230.0f, 12.0f);
    g.fillRoundedRectangle (560.0f, 250.0f, 220.0f, 230.0f, 12.0f);

    g.setColour (juce::Colour::fromRGB (255, 170, 60));
    g.setFont (juce::Font (16.0f, juce::Font::bold));

    g.drawText ("LOW SHELF",   50,  260, 200, 30, juce::Justification::centred);
    g.drawText ("MID PEAK",    310, 260, 200, 30, juce::Justification::centred);
    g.drawText ("HIGH SHELF",  570, 260, 200, 30, juce::Justification::centred);
}

void SimpleRealtimeEQAudioProcessorEditor::resized()
{
    const int sliderWidth = 80;
    const int sliderHeight = 90;
    const int labelHeight = 20;

    // =========================
    // Saturation row
    // =========================

    const int satY = 115;
    const int satStartX = 285;
    const int satSpacing = 90;

    satDriveSlider.setBounds (satStartX, satY, sliderWidth, sliderHeight);
    satDriveLabel.setBounds  (satStartX, satY + sliderHeight, sliderWidth, labelHeight);

    satBiasSlider.setBounds (satStartX + satSpacing, satY, sliderWidth, sliderHeight);
    satBiasLabel.setBounds  (satStartX + satSpacing, satY + sliderHeight, sliderWidth, labelHeight);

    satMixSlider.setBounds (satStartX + satSpacing * 2, satY, sliderWidth, sliderHeight);
    satMixLabel.setBounds  (satStartX + satSpacing * 2, satY + sliderHeight, sliderWidth, labelHeight);

    // =========================
    // EQ columns
    // =========================

    const int eqY = 310;

    const int lowX = 50;
    const int midX = 310;
    const int highX = 570;

    auto placeColumn = [&] (int x,
                            juce::Slider& slider1, juce::Label& label1,
                            juce::Slider& slider2, juce::Label& label2,
                            juce::Slider& slider3, juce::Label& label3)
    {
        slider1.setBounds (x, eqY, sliderWidth, sliderHeight);
        label1.setBounds  (x, eqY + sliderHeight, sliderWidth, labelHeight);

        slider2.setBounds (x + 70, eqY, sliderWidth, sliderHeight);
        label2.setBounds  (x + 70, eqY + sliderHeight, sliderWidth, labelHeight);

        slider3.setBounds (x + 140, eqY, sliderWidth, sliderHeight);
        label3.setBounds  (x + 140, eqY + sliderHeight, sliderWidth, labelHeight);
    };

    placeColumn (lowX,
                 lowFreqSlider, lowFreqLabel,
                 lowGainSlider, lowGainLabel,
                 lowSSlider,    lowSLabel);

    placeColumn (midX,
                 midFreqSlider, midFreqLabel,
                 midGainSlider, midGainLabel,
                 midQSlider,    midQLabel);

    placeColumn (highX,
                 highFreqSlider, highFreqLabel,
                 highGainSlider, highGainLabel,
                 highSSlider,    highSLabel);
}

void SimpleRealtimeEQAudioProcessorEditor::setupSlider (juce::Slider& slider)
{
    slider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 70, 20);
    slider.setColour (juce::Slider::rotarySliderFillColourId, juce::Colours::orange);
    slider.setColour (juce::Slider::thumbColourId, juce::Colours::white);

    addAndMakeVisible (slider);
}

void SimpleRealtimeEQAudioProcessorEditor::setupLabel (juce::Label& label, const juce::String& text)
{
    label.setText (text, juce::dontSendNotification);
    label.setJustificationType (juce::Justification::centred);
    label.setColour (juce::Label::textColourId, juce::Colours::white);

    addAndMakeVisible (label);
}
// GitHub test
