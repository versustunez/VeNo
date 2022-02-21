#include <VeNo/Sound/Tables/TableGenerator.h>
#include <cassert>
#include <cmath>

namespace VeNo::Audio {

WaveTableGroup *WaveTableGenerator::getGroup(int id) {
  return m_waveTables[id];
}

WaveTableGroup *WaveTableGenerator::getGroup(double wavePosition) {
  int index = (int)std::floor(wavePosition * m_elements);
  assert(index < WAVETABLE_SLOTS);
  return m_waveTables[index];
}

void WaveTableGenerator::init() {
  std::lock_guard<std::mutex> lockGuard(m_guard);
  if (m_isInit)
    return;
  // create first tables
  m_isInit = true;
}

void WaveTableGenerator::cleanTables() {
  std::lock_guard<std::mutex> lockGuard(m_guard);
  if (!m_isInit)
    return;
  for (auto table : m_waveTables) {
    delete table;
    table = nullptr;
  }
}

void WaveTableGenerator::setGroup(int index, WaveTableGroup *group) {
  std::lock_guard<std::mutex> lockGuard(m_guard);
  assert(m_elements > index);
  if (m_replaceGroup[index] != nullptr && m_replaceGroup[index] != group)
    delete m_replaceGroup[index];
  m_replaceGroup[index] = group;
  m_hasReplacement = true;
}

void WaveTableGenerator::deleteGroup(int index) {
  for (int i = index; i < WAVETABLE_SLOTS - 1; i++)
    m_waveTables[i] = m_waveTables[i + 1];
  m_elements--;
}

void WaveTableGenerator::addGroup(WaveTableGroup *group) {
  m_waveTables[m_elements++] = group;
}

void WaveTableGenerator::replace() {
  std::lock_guard<std::mutex> lockGuard(m_guard);
  hasNewData = false;
  if (!m_hasReplacement)
    return;
  for (int i = 0; i < m_elements; ++i) {
    if (m_replaceGroup[i] == nullptr)
      continue;
    hasNewData = true;
    delete m_waveTables[i];
    m_waveTables[i] = m_replaceGroup[i];
    m_replaceGroup[i] = nullptr;
  }
  m_hasReplacement = false;
}
} // namespace VeNo::Audio