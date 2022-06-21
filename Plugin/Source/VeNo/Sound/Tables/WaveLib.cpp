#include "WaveLib.h"

#include "TableCreator.h"
#include "VeNo/Core/Config.h"

namespace VeNo::Audio {
Mutex WaveLib::createGuard{};
WaveLib &WaveLib::Get() {
  Guard lockGuard(createGuard);
  static WaveLib instance;
  return instance;
}
WaveLib::~WaveLib() { Clear(); }

void WaveLib::Clear() {
  for (auto *wave : m_WaveTables) {
    delete wave;
  }
}
void WaveLib::CreateAll() {
  auto& config = Core::Config::get();
  if (config.sampleRate != m_SampleRate) {
    m_SampleRate = config.sampleRate;
    Clear();
    // Create WaveTables ;)
    TableCreator::CreateAll();
  }
}
WaveTable *WaveLib::GetWaveTable(int index) {
  return m_WaveTables[index];
}
void WaveLib::SetWaveTable(int index, WaveTable *table) {
  delete m_WaveTables[index];
  m_WaveTables[index] = table;
}
} // namespace VeNo::Sound