#pragma once
#include "Structs.h"

#include <VeNo/Definitions.h>
#include <VeNo/TypeDefs.h>

namespace VeNo::Audio {
using RefGroup = Ref<WaveTableGroup>;
class WaveTableLib {
public:
  WaveTableLib() { createGroup(); };
  RefGroup createGroup(bool add = true);
  void addGroup(const RefGroup &group);
  void replaceGroup(size_t index, const RefGroup &group);
  void remove(long index);
  void resetGroup(size_t index);
  RefGroup &getGroup(double wavePosition);
  RefGroup &getGroup(size_t index) { return m_groups[index]; };

  size_t size() { return m_groups.size(); }
  [[nodiscard]] bool isOutdated() const {
    return m_isDirty;
  } // say we need to fetch a new Group because something has changed :O
  void removeOutdated() { m_isDirty = false; }
  void setInstanceId(size_t id) { m_instance = id; }

  size_t id() { return m_instance; }

  void regenerateAll();

protected:
  // a bit overhead to have a Ref Counted Table but its worth!
  // so we can copy it on change to the OSC itself and will not crash on GUI
  // Change... and we don't have to use a Double Buffered Structure for this
  Vector<RefGroup> m_groups{};
  size_t m_instance{0};
  bool m_isDirty{false};
};
} // namespace VeNo::Audio