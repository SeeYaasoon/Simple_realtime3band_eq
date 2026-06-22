//
//  CustomLookAndFeel.h
//  SimpleRealtimeEQ - Standalone Plugin
//
//  Created by Stanislav Hrebnev on 08.06.2026.
//

#pragma once

#include <JuceHeader.h>

class CustomLookAndFeel : public juce::LookAndFeel_V4
{
public:
    CustomLookAndFeel()
    {
        setColour (juce::Slider::thumbColourId, juce::Colours::white);
        setColour (juce::Slider::rotarySliderFillColourId, juce::Colour::fromRGB (255, 170, 60));
        setColour (juce::Slider::rotarySliderOutlineColourId, juce::Colour::fromRGB (45, 45, 50));
        setColour (juce::Slider::textBoxTextColourId, juce::Colours::white);
        setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour::fromRGB (25, 25, 28));
        setColour (juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    }

    void drawRotarySlider (juce::Graphics& g,
                           int x,
                           int y,
                           int width,
                           int height,
                           float sliderPosProportional,
                           float rotaryStartAngle,
                           float rotaryEndAngle,
                           juce::Slider& slider) override
    {
        juce::ignoreUnused (slider);

        auto bounds = juce::Rectangle<float> (static_cast<float> (x),
                                              static_cast<float> (y),
                                              static_cast<float> (width),
                                              static_cast<float> (height));

        bounds = bounds.reduced (8.0f);

        const float radius = juce::jmin (bounds.getWidth(), bounds.getHeight()) * 0.5f;
        const float centreX = bounds.getCentreX();
        const float centreY = bounds.getCentreY();

        const float lineWidth = 5.0f;
        const float arcRadius = radius - lineWidth * 0.5f;

        const float angle = rotaryStartAngle
            + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);

        juce::Path backgroundArc;
        backgroundArc.addCentredArc (centreX,
                                     centreY,
                                     arcRadius,
                                     arcRadius,
                                     0.0f,
                                     rotaryStartAngle,
                                     rotaryEndAngle,
                                     true);

        g.setColour (juce::Colour::fromRGB (50, 50, 56));
        g.strokePath (backgroundArc,
                      juce::PathStrokeType (lineWidth,
                                            juce::PathStrokeType::curved,
                                            juce::PathStrokeType::rounded));

        juce::Path valueArc;
        valueArc.addCentredArc (centreX,
                                centreY,
                                arcRadius,
                                arcRadius,
                                0.0f,
                                rotaryStartAngle,
                                angle,
                                true);

        g.setColour (juce::Colour::fromRGB (255, 170, 60));
        g.strokePath (valueArc,
                      juce::PathStrokeType (lineWidth,
                                            juce::PathStrokeType::curved,
                                            juce::PathStrokeType::rounded));

        auto knobBounds = bounds.reduced (18.0f);

        g.setColour (juce::Colour::fromRGB (32, 32, 36));
        g.fillEllipse (knobBounds);

        g.setColour (juce::Colour::fromRGB (80, 80, 88));
        g.drawEllipse (knobBounds, 1.5f);

        const float pointerLength = radius * 0.42f;
        const float pointerThickness = 3.0f;

        juce::Path pointer;
        pointer.addRoundedRectangle (-pointerThickness * 0.5f,
                                     -pointerLength,
                                     pointerThickness,
                                     pointerLength,
                                     pointerThickness * 0.5f);

        pointer.applyTransform (juce::AffineTransform::rotation (angle)
                                    .translated (centreX, centreY));

        g.setColour (juce::Colours::white);
        g.fillPath (pointer);
    }
};
