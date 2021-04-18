#pragma once

#include <JuceHeader.h>
#include <VeNo/Sound/Generation/WaveTables/Structs.h>

namespace VeNo::Audio::Waves
{
class WaveTableGenerator
{
private:
    static constexpr int numWaveTableSlots = WAVETABLES;

public:
    WaveTableGroup* m_waveTables[numWaveTableSlots]{};
    static WaveTableGenerator& getInstance()
    {
        static WaveTableGenerator instance;
        return instance;
    }
    WaveTableGroup* getGroup (int id);
    void init();
    void cleanTables();
    void setGroup (WaveForms index, WaveTableGroup* group);

protected:
    std::mutex guard;
    bool m_isInit = false;
    WaveTableGenerator() = default;
    ~WaveTableGenerator() = default;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveTableGenerator)
};
} // namespace VeNo::Audio::Waves
