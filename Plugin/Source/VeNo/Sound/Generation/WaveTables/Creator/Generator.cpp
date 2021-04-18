#include <VUtils/Logging.h>
#include <VeNo/Sound/Generation/WaveTables/Creator/Generator.h>
namespace VeNo::Audio::Waves
{
void WaveTableGenerator::init()
{
    std::lock_guard<std::mutex> lockGuard(guard);
    if (m_isInit) return;
    DBGN("Create WaveTables")
    m_isInit = true;
}
WaveTableGroup* WaveTableGenerator::getGroup (int id)
{
    return m_waveTables[id];
}
void WaveTableGenerator::cleanTables()
{
    std::lock_guard<std::mutex> lockGuard(guard);
    if (!m_isInit) return;
    DBGN("Destroy WaveTables")
    // We Guard to avoid double cleanup and more! ;)
    for (auto table : m_waveTables)
    {
        if (table == nullptr) continue;
        delete table;
        table = nullptr;
    }
    m_isInit = false;
}
void WaveTableGenerator::setGroup (WaveForms index, WaveTableGroup* group)
{
    m_waveTables[index] = group;
}
} // namespace VeNo::Audio::Waves