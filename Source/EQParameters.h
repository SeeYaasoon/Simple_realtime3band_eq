//
//  EQParameters.h
//  SimpleRealtimeEQ - Standalone Plugin
//
//  Created by Stanislav Hrebnev on 07.06.2026.
//

#pragma once

namespace EQParameterIDs
{
    constexpr const char* lowFreq  = "lowFreq";
    constexpr const char* lowGain  = "lowGain";
    constexpr const char* lowS     = "lowS";

    constexpr const char* midFreq  = "midFreq";
    constexpr const char* midGain  = "midGain";
    constexpr const char* midQ     = "midQ";

    constexpr const char* highFreq = "highFreq";
    constexpr const char* highGain = "highGain";
    constexpr const char* highS    = "highS";
    constexpr const char* satDrive = "satDrive";
    constexpr const char* satBias  = "satBias";
    constexpr const char* satMix   = "satMix";
}
