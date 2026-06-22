//
//  Biquad.h
//  SimpleRealtimeEQ - Standalone Plugin
//
//  Created by Stanislav Hrebnev on 07.06.2026.
//

#pragma once

#include <JuceHeader.h>

class Biquad
{
public:
    Biquad() = default;

    void reset()
    {
        z1 = 0.0f;
        z2 = 0.0f;
    }

    void setCoefficients(float newB0,
                         float newB1,
                         float newB2,
                         float newA1,
                         float newA2)
    {
        b0 = newB0;
        b1 = newB1;
        b2 = newB2;
        a1 = newA1;
        a2 = newA2;
    }

    float processSample(float x)
    {
        const float y = b0 * x + z1;

        z1 = b1 * x - a1 * y + z2;
        z2 = b2 * x - a2 * y;

        return y;
    }

private:
    float b0 = 1.0f;
    float b1 = 0.0f;
    float b2 = 0.0f;

    float a1 = 0.0f;
    float a2 = 0.0f;

    float z1 = 0.0f;
    float z2 = 0.0f;
};
