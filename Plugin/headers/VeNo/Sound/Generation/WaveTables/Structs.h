#pragma once
#include <VeNo/Definitions.h>
#include <cstdint>
namespace VeNo::Audio::Waves
{
enum WaveForms
{
    SAW = 0,
    SINE,
    COSINE,
    SQUARE,
    TRIANGLE,
    DIRTY_SAW,
    DIRTY_SQUARE,
    SYNTH_ONE,
    SYNTH_TWO,
    VENOX,
    // Noise can come from NoiseGenerator Directly! it will generate some Noise each Step if enabled but we will let the WaveTables in they are "PRE Generated" :)
    PRE_NOISE,
    PRE_SMOOTH_NOISE,
    PRE_SMOOTHER_NOISE,
};
struct WaveTableObject
{
    double topFreq = 0.0;
    uint32_t tableLen = 0;
    double* waveTable = nullptr;
    ~WaveTableObject();
};
struct WaveTableGroup
{
    static constexpr int numWaveTableSlots = WAVETABLE_SLOTS;
    WaveTableObject m_WaveTables[numWaveTableSlots];
    int m_numWaveTables = 0;
};
} // namespace VeNo::Audio::Waves