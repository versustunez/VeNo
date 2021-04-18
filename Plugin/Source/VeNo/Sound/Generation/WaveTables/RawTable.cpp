#include <VeNo/Sound/Generation/WaveTables/RawTable.h>
#include <VeNo/Sound/Generation/WaveTables/Creator/Generator.h>

namespace VeNo::Audio::Waves
{
void RawTable::cleanUp()
{
    m_freqWaveRe.clear();
    m_freqWaveIm.clear();
    m_tempIm.clear();
    m_tempRe.clear();
}

void RawTable::setSize (size_t size)
{
    cleanUp();
    m_freqWaveIm.resize (size);
    m_freqWaveRe.resize (size);
    m_tempIm.resize (size);
    m_tempRe.resize (size);
    m_len = size;
}
void RawTable::cleanUpTemp()
{
    m_tempIm.clear();
    m_tempRe.clear();
    m_tempIm.resize (m_len);
    m_tempRe.resize (m_len);
}
void RawTable::finish() const
{
    WaveTableGenerator::getInstance().setGroup(m_index, m_group);
}
} // namespace VeNo::Audio::Waves