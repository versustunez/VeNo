#pragma once
#include <VeNo/Definitions.h>
#include <VeNo/Sound/Tables/WaveLib.h>
#include <cassert>

namespace VeNo::Audio {
struct Channel {
  double left{0};
  double right{0};
};

struct WaveHolder {
  static constexpr int size = OSCILLATORS + LFOS + 1;
  explicit WaveHolder(InstanceID id);
  WaveTableLib *current() { return generators[m_currentTable].get(); }
  Scope<WaveTableLib> generators[size]; // + 1 because of the DIST_MODULE
  void setCurrentTable(size_t currentTable) {
    assert(currentTable < size);
    m_currentTable = currentTable;
  }

  size_t currentTable() { return m_currentTable; }

protected:
  size_t m_currentTable{0};
};
} // namespace VeNo::Audio