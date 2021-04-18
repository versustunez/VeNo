#pragma once

#include <VeNo/Sound/Generation/WaveTables/Structs.h>
#include <vector>

namespace VeNo::Audio::Waves
{
    struct RawTable
    {
        WaveForms m_index;
        WaveTableGroup* m_group = new WaveTableGroup();
        std::vector<double> m_freqWaveRe;
        std::vector<double> m_freqWaveIm;
        std::vector<double> m_tempRe;
        std::vector<double> m_tempIm;
        size_t m_len = 0;
        bool m_isRaw = false; // needed for Sine, Cosine...

        ~RawTable() = default;
        virtual void generate() = 0;
        void setSize(size_t size);
        void cleanUp();
        void cleanUpTemp();
        void finish() const;
    };
}