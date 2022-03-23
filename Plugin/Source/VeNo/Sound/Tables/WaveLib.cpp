#include "VeNo/Sound/Tables/WaveTable.h"

#include <VeNo/Sound/Tables/WaveLib.h>
#include <cassert>
#include <cmath>

namespace VeNo::Audio {
RefGroup WaveTableLib::createGroup(bool add) {
  RefGroup group = CreateRef<WaveTableGroup>();
  auto &points = group->uiPoints;
  points.addCurvedPoint(0, 1, -1, {}, true);
  points.addPoint(1, 0, 1, true);
  points.updateNeighbours();
  points.updateCurved();
  if (add) {
    addGroup(group);
  }
  Audio::WaveTableCreator::generate(group->uiPoints.points, group.get());
  return group;
}

void WaveTableLib::addGroup(const RefGroup &group) {
  m_groups.push_back(group);
  group->index = m_groups.size() - 1;
  m_isDirty = true;
}

void WaveTableLib::replaceGroup(size_t index, const RefGroup &group) {
  assert(m_groups.size() > index);
  m_groups[index] = group;
  m_isDirty = true;
}

void WaveTableLib::remove(long index) {
  m_groups.erase(m_groups.begin() + index);
  m_isDirty = true;
}

RefGroup &WaveTableLib::getGroup(double wavePosition) {
  auto index = (size_t)std::floor(wavePosition * (double)m_groups.size());
  return m_groups[index];
}

void WaveTableLib::regenerateAll() {
  for (size_t i = 0; i < m_groups.size(); ++i) {
    auto &oldGroup = m_groups[i];
    auto group = CreateRef<Audio::WaveTableGroup>();
    group->uiPoints = oldGroup->uiPoints;
    group->uiPoints.updateNeighbours();
    Audio::WaveTableCreator::generate(group->uiPoints.points, group.get());
    replaceGroup(i, group);
  }
}
void WaveTableLib::resetGroup(size_t index) {
  auto group = createGroup(false);
  replaceGroup(index, group);
}
} // namespace VeNo::Audio