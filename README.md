# Simple Real-Time 3-Band EQ

A real-time audio effect plugin developed in C++ using the JUCE framework.

The project combines a three-band parametric equalizer with a nonlinear preamp saturation stage. It was created as a practical DSP and audio plugin development project.

## Features

### Preamp saturation

The input signal is processed by a nonlinear saturation stage before the equalizer.

Available controls:

* **Drive** — controls the intensity of the nonlinear distortion
* **Bias** — introduces asymmetric saturation
* **Mix** — blends the processed and unprocessed signals

The saturation stage uses a hyperbolic tangent transfer function and includes:

* input-level-dependent dynamic bias
* automatic output gain compensation
* DC compensation
* first-order DC blocking filter
* dry/wet mixing

### Three-band equalizer

The plugin contains three independently adjustable EQ sections:

* **Low Shelf**

  * frequency
  * gain
  * slope

* **Mid Peak**

  * center frequency
  * gain
  * Q factor

* **High Shelf**

  * frequency
  * gain
  * slope

Each section is implemented using a second-order biquad IIR filter.

## Signal Processing Chain

```text
Audio Input
    │
    ▼
Input RMS Estimation
    │
    ▼
Dynamic Preamp Saturation
    │
    ▼
DC Blocking Filter
    │
    ▼
Low-Shelf Filter
    │
    ▼
Mid-Peak Filter
    │
    ▼
High-Shelf Filter
    │
    ▼
Output Limiting
    │
    ▼
Audio Output
```

## DSP Implementation

The audio signal is processed sample by sample inside the JUCE `processBlock()` callback.

The equalizer uses custom biquad filters implemented with the following difference equation:

```text
y[n] = b0·x[n] + b1·x[n−1] + b2·x[n−2]
       − a1·y[n−1] − a2·y[n−2]
```

Filter coefficients are recalculated from the current frequency, gain, slope and Q parameters.

Separate filter states are maintained for the left and right audio channels.

The saturation stage uses a nonlinear transfer function based on:

```text
y = tanh(drive · (x + bias))
```

The bias is adjusted according to the smoothed RMS level of the input block, creating a simple level-dependent nonlinear response.

## User Interface

The graphical interface contains separate control sections for:

* preamp saturation
* low-frequency shelf
* mid-frequency peak filter
* high-frequency shelf

The interface parameters are connected to the audio processor through JUCE `AudioProcessorValueTreeState` slider attachments.

## Technologies

* C++
* JUCE
* Digital Signal Processing
* Biquad IIR filters
* Nonlinear audio processing
* Real-time audio processing
* Xcode
* Git

## Project Structure

```text
Simple_realtime3band_eq/
├── Source/
│   ├── Biquad.h
│   ├── CustomLookAndFeel.h
│   ├── EQParameters.h
│   ├── PluginEditor.cpp
│   ├── PluginEditor.h
│   ├── PluginProcessor.cpp
│   └── PluginProcessor.h
├── SimpleRealtimeEQ.jucer
├── JuceLibraryCode/
└── .gitignore
```

## Requirements

* macOS
* Xcode
* JUCE framework
* Projucer

The project currently uses an Xcode exporter configured through the included `.jucer` project file.

## Building the Project

1. Clone the repository:

```bash
git clone https://github.com/SeeYaasoon/Simple_realtime3band_eq.git
```

2. Open the project folder:

```bash
cd Simple_realtime3band_eq
```

3. Open `SimpleRealtimeEQ.jucer` in Projucer.

4. Set the correct path to the local JUCE `modules` directory.

5. Select **Save Project and Open in IDE**.

6. Build the desired target in Xcode.

```text
Product → Build
```

or press:

```text
Command + B
```

## Parameters

| Section    | Parameter | Function                                |
| ---------- | --------- | --------------------------------------- |
| Saturation | Drive     | Controls nonlinear distortion intensity |
| Saturation | Bias      | Controls saturation asymmetry           |
| Saturation | Mix       | Controls the dry/wet balance            |
| Low Shelf  | Frequency | Sets the low-shelf cutoff frequency     |
| Low Shelf  | Gain      | Boosts or attenuates low frequencies    |
| Low Shelf  | Slope     | Controls the shelf transition shape     |
| Mid Peak   | Frequency | Sets the center frequency               |
| Mid Peak   | Gain      | Boosts or attenuates the selected band  |
| Mid Peak   | Q         | Controls the bandwidth                  |
| High Shelf | Frequency | Sets the high-shelf cutoff frequency    |
| High Shelf | Gain      | Boosts or attenuates high frequencies   |
| High Shelf | Slope     | Controls the shelf transition shape     |

## Purpose

This project was developed to practise:

* real-time audio plugin architecture
* digital filter implementation
* calculation of biquad coefficients
* nonlinear signal processing
* parameter management in JUCE
* audio processor and graphical interface integration

## Current Limitations

* Plugin parameter state saving and restoration are not yet implemented.
* The project currently contains only an Xcode/macOS exporter.
* Automated tests and continuous integration are not yet included.
* Parameter smoothing can be improved to prevent abrupt coefficient changes.
* The build process requires a locally installed JUCE framework.

## Possible Future Improvements

* parameter smoothing
* frequency-response visualization
* input and output level meters
* preset management
* plugin state serialization
* bypass controls for individual processing sections
* oversampling for the saturation stage
* automated builds with GitHub Actions
* Windows and Linux exporters

## Author

Audio DSP and plugin development project by SeeYaasoon.
