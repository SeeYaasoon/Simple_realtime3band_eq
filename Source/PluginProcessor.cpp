/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SimpleRealtimeEQAudioProcessor::SimpleRealtimeEQAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
       apvts (*this, nullptr, "Parameters", createParameterLayout())
#else
     : apvts (*this, nullptr, "Parameters", createParameterLayout())
#endif
{
}

SimpleRealtimeEQAudioProcessor::~SimpleRealtimeEQAudioProcessor()
{
}
juce::AudioProcessorValueTreeState::ParameterLayout
SimpleRealtimeEQAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back (std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID { EQParameterIDs::lowFreq, 1 },
        "Low Frequency",
        juce::NormalisableRange<float> (20.0f, 500.0f, 1.0f, 0.5f),
        150.0f));

    params.push_back (std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID { EQParameterIDs::lowGain, 1 },
        "Low Gain",
        juce::NormalisableRange<float> (-24.0f, 24.0f, 0.1f),
        -10.0f));

    params.push_back (std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID { EQParameterIDs::lowS, 1 },
        "Low Slope",
        juce::NormalisableRange<float> (0.2f, 2.0f, 0.01f),
        1.0f));

    params.push_back (std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID { EQParameterIDs::midFreq, 1 },
        "Mid Frequency",
        juce::NormalisableRange<float> (100.0f, 10000.0f, 1.0f, 0.5f),
        3000.0f));

    params.push_back (std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID { EQParameterIDs::midGain, 1 },
        "Mid Gain",
        juce::NormalisableRange<float> (-24.0f, 24.0f, 0.1f),
        -6.0f));

    params.push_back (std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID { EQParameterIDs::midQ, 1 },
        "Mid Q",
        juce::NormalisableRange<float> (0.1f, 10.0f, 0.01f),
        0.6f));

    params.push_back (std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID { EQParameterIDs::highFreq, 1 },
        "High Frequency",
        juce::NormalisableRange<float> (1000.0f, 18000.0f, 1.0f, 0.5f),
        8000.0f));

    params.push_back (std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID { EQParameterIDs::highGain, 1 },
        "High Gain",
        juce::NormalisableRange<float> (-24.0f, 24.0f, 0.1f),
        -10.0f));

    params.push_back (std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID { EQParameterIDs::highS, 1 },
        "High Slope",
        juce::NormalisableRange<float> (0.2f, 2.0f, 0.01f),
        1.0f));
    params.push_back (std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID { EQParameterIDs::satDrive, 1 },
        "Saturation Drive",
        juce::NormalisableRange<float> (1.0f, 20.0f, 0.01f, 0.5f),
        1.0f));

    params.push_back (std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID { EQParameterIDs::satBias, 1 },
        "Saturation Bias",
        juce::NormalisableRange<float> (-0.5f, 0.5f, 0.001f),
        0.0f));

    params.push_back (std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID { EQParameterIDs::satMix, 1 },
        "Saturation Mix",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f),
        1.0f));

    return { params.begin(), params.end() };
}

//==============================================================================
const juce::String SimpleRealtimeEQAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SimpleRealtimeEQAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SimpleRealtimeEQAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SimpleRealtimeEQAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SimpleRealtimeEQAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SimpleRealtimeEQAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SimpleRealtimeEQAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SimpleRealtimeEQAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SimpleRealtimeEQAudioProcessor::getProgramName (int index)
{
    return {};
}

void SimpleRealtimeEQAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SimpleRealtimeEQAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::ignoreUnused (samplesPerBlock);

    currentSampleRate = sampleRate;

    lowShelfL.reset();
    midPeakL.reset();
    highShelfL.reset();

    lowShelfR.reset();
    midPeakR.reset();
    highShelfR.reset();
    dcPrevXL = 0.0f;
    dcPrevYL = 0.0f;

    dcPrevXR = 0.0f;
    dcPrevYR = 0.0f;

    updateFilters();
    inputRMS = 0.0f;
}

void SimpleRealtimeEQAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SimpleRealtimeEQAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void SimpleRealtimeEQAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                                   juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages);
    juce::ScopedNoDenormals noDenormals;

    updateFilters();

    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();

    for (int channel = totalNumInputChannels; channel < totalNumOutputChannels; ++channel)
        buffer.clear (channel, 0, buffer.getNumSamples());

    const int numSamples = buffer.getNumSamples();
    const int numChannels = juce::jmin (2, buffer.getNumChannels());

    // ==============================================================================
    // 1. СЧИТАЕМ RMS ТЕКУЩЕГО БЛОКА (прямо средствами JUCE — это очень быстро)
    // ==============================================================================
    float blockRMS = 0.0f;
    if (numChannels > 0 && numSamples > 0)
    {
        for (int i = 0; i < numChannels; ++i)
        {
            // Функция getRMSLevel возвращает среднеквадратичное значение для канала
            blockRMS += buffer.getRMSLevel(i, 0, numSamples);
        }
        blockRMS /= static_cast<float>(numChannels); // Берем среднее между левым и правым
    }

    // ==============================================================================
    // 2. ПЛАВНО СГЛАЖИВАЕМ RMS (чтобы не было щелчков на стыках блоков)
    // inputRMS — это та переменная, которую мы добавили в класс (.h файл)
    // ==============================================================================
    // Коэффициент 0.8f сглаживает реакцию между блоками.
    // Чем ближе к 1.0f, тем медленнее и плавнее будет дышать автоматика.
    inputRMS = 0.8f * inputRMS + 0.2f * blockRMS;

    // Получаем параметры из ручек один раз на весь блок (оптимизация процессора!)
    const float baseBias = apvts.getRawParameterValue (EQParameterIDs::satBias)->load();
    const float drive    = apvts.getRawParameterValue (EQParameterIDs::satDrive)->load();
    const float mix      = apvts.getRawParameterValue (EQParameterIDs::satMix)->load();

    // Считаем авто-гейн на основе текущего drive
    const float autoGain = 1.0f / std::sqrt(drive);

    // ==============================================================================
    // 3. ОСНОВНОЙ ЦИКЛ ОБРАБОТКИ
    // ==============================================================================
    for (int channel = 0; channel < numChannels; ++channel)
    {
        auto* data = buffer.getWritePointer (channel);

        Biquad& lowShelf  = channel == 0 ? lowShelfL  : lowShelfR;
        Biquad& midPeak   = channel == 0 ? midPeakL   : midPeakR;
        Biquad& highShelf = channel == 0 ? highShelfL : highShelfR;

        float& dcPrevX = channel == 0 ? dcPrevXL : dcPrevXR;
        float& dcPrevY = channel == 0 ? dcPrevYL : dcPrevYR;

        // Динамический Bias теперь одинаковый и стабильный для всего этого блока сэмплов!
        float dynamicBias = baseBias + (inputRMS * 0.2f);
        
        // Предрасчет статической компенсации для этого bias
        const float dcCompensation = std::tanh (drive * dynamicBias);

        for (int sample = 0; sample < numSamples; ++sample)
        {
            float x = data[sample];
            const float dry = x;

            // Сатурация с динамическим смещением
            float saturated = std::tanh (drive * (x + dynamicBias)) - dcCompensation;

            // Авто-компенсация громкости
            saturated *= autoGain;

            // Dry/Wet Mix
            x = dry * (1.0f - mix) + saturated * mix;

            // DC blocker:
            // y[n] = x[n] - x[n-1] + R * y[n-1]
            // R близко к 1, поэтому убирается только очень низкий DC/инфраниз.
            const float R = 0.995f;
            const float dcBlocked = x - dcPrevX + R * dcPrevY;

            dcPrevX = x;
            dcPrevY = dcBlocked;

            x = dcBlocked;

            // EQ после сатурации.
            x = lowShelf.processSample (x);
            x = midPeak.processSample (x);
            x = highShelf.processSample (x);

            data[sample] = juce::jlimit (-0.98f, 0.98f, x);
        }
    }
}

//==============================================================================
bool SimpleRealtimeEQAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SimpleRealtimeEQAudioProcessor::createEditor()
{
    return new SimpleRealtimeEQAudioProcessorEditor (*this);
}

//==============================================================================
void SimpleRealtimeEQAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SimpleRealtimeEQAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
void SimpleRealtimeEQAudioProcessor::updateFilters()
{
    const float fs = static_cast<float> (currentSampleRate);
    const float nyquist = fs * 0.5f;

    auto getParam = [this] (const char* id)
    {
        return apvts.getRawParameterValue (id)->load();
    };

    const float lowFreq = juce::jlimit (20.0f, nyquist * 0.95f, getParam (EQParameterIDs::lowFreq));
    const float lowGain = getParam (EQParameterIDs::lowGain);
    const float lowS    = juce::jmax (0.05f, getParam (EQParameterIDs::lowS));

    const float midFreq = juce::jlimit (20.0f, nyquist * 0.95f, getParam (EQParameterIDs::midFreq));
    const float midGain = getParam (EQParameterIDs::midGain);
    const float midQ    = juce::jmax (0.05f, getParam (EQParameterIDs::midQ));

    const float highFreq = juce::jlimit (20.0f, nyquist * 0.95f, getParam (EQParameterIDs::highFreq));
    const float highGain = getParam (EQParameterIDs::highGain);
    const float highS    = juce::jmax (0.05f, getParam (EQParameterIDs::highS));

    auto setPeakingEQ = [] (Biquad& filter, float Fc, float gainDB, float Q, float sampleRate)
    {
        const float A = std::pow (10.0f, gainDB / 40.0f);
        const float w0 = 2.0f * juce::MathConstants<float>::pi * Fc / sampleRate;
        const float alpha = std::sin (w0) / (2.0f * Q);

        const float b0 = 1.0f + alpha * A;
        const float b1 = -2.0f * std::cos (w0);
        const float b2 = 1.0f - alpha * A;

        const float a0 = 1.0f + alpha / A;
        const float a1 = -2.0f * std::cos (w0);
        const float a2 = 1.0f - alpha / A;

        filter.setCoefficients (b0 / a0,
                                b1 / a0,
                                b2 / a0,
                                a1 / a0,
                                a2 / a0);
    };

    auto setLowShelfEQ = [] (Biquad& filter, float Fc, float gainDB, float S, float sampleRate)
    {
        const float A = std::pow (10.0f, gainDB / 40.0f);
        const float w0 = 2.0f * juce::MathConstants<float>::pi * Fc / sampleRate;

        const float alpha = std::sin (w0) / 2.0f
            * std::sqrt ((A + 1.0f / A) * (1.0f / S - 1.0f) + 2.0f);

        const float cosw0 = std::cos (w0);
        const float beta = 2.0f * std::sqrt (A) * alpha;

        const float b0 = A * ((A + 1.0f) - (A - 1.0f) * cosw0 + beta);
        const float b1 = 2.0f * A * ((A - 1.0f) - (A + 1.0f) * cosw0);
        const float b2 = A * ((A + 1.0f) - (A - 1.0f) * cosw0 - beta);

        const float a0 = (A + 1.0f) + (A - 1.0f) * cosw0 + beta;
        const float a1 = -2.0f * ((A - 1.0f) + (A + 1.0f) * cosw0);
        const float a2 = (A + 1.0f) + (A - 1.0f) * cosw0 - beta;

        filter.setCoefficients (b0 / a0,
                                b1 / a0,
                                b2 / a0,
                                a1 / a0,
                                a2 / a0);
    };

    auto setHighShelfEQ = [] (Biquad& filter, float Fc, float gainDB, float S, float sampleRate)
    {
        const float A = std::pow (10.0f, gainDB / 40.0f);
        const float w0 = 2.0f * juce::MathConstants<float>::pi * Fc / sampleRate;

        const float alpha = std::sin (w0) / 2.0f
            * std::sqrt ((A + 1.0f / A) * (1.0f / S - 1.0f) + 2.0f);

        const float cosw0 = std::cos (w0);
        const float beta = 2.0f * std::sqrt (A) * alpha;

        const float b0 = A * ((A + 1.0f) + (A - 1.0f) * cosw0 + beta);
        const float b1 = -2.0f * A * ((A - 1.0f) + (A + 1.0f) * cosw0);
        const float b2 = A * ((A + 1.0f) + (A - 1.0f) * cosw0 - beta);

        const float a0 = (A + 1.0f) - (A - 1.0f) * cosw0 + beta;
        const float a1 = 2.0f * ((A - 1.0f) - (A + 1.0f) * cosw0);
        const float a2 = (A + 1.0f) - (A - 1.0f) * cosw0 - beta;

        filter.setCoefficients (b0 / a0,
                                b1 / a0,
                                b2 / a0,
                                a1 / a0,
                                a2 / a0);
    };

    setLowShelfEQ  (lowShelfL,  lowFreq,  lowGain,  lowS,  fs);
    setPeakingEQ   (midPeakL,   midFreq,  midGain,  midQ,  fs);
    setHighShelfEQ (highShelfL, highFreq, highGain, highS, fs);

    setLowShelfEQ  (lowShelfR,  lowFreq,  lowGain,  lowS,  fs);
    setPeakingEQ   (midPeakR,   midFreq,  midGain,  midQ,  fs);
    setHighShelfEQ (highShelfR, highFreq, highGain, highS, fs);
}
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SimpleRealtimeEQAudioProcessor();
}
