#pragma once
#include "Structs.h"

#include <VeNo/Definitions.h>
#include <VeNo/TypeDefs.h>

namespace VeNo::Audio {
class WaveTableGenerator {
public:
  WaveTableGroup *getGroup(int id);
  WaveTableGroup *getGroup(double wavePosition);
  void init();
  void cleanTables();
  void setGroup(int index, WaveTableGroup *group);
  void deleteGroup(int index);
  void addGroup(WaveTableGroup *group);
  void replace();
  void setInstanceId(size_t id) { m_instanceID = id; }

public:
  WaveTableGroup *m_waveTables[WAVETABLE_SLOTS]{};
  WaveTableGroup *m_replaceGroup[WAVETABLE_SLOTS]{};
  bool hasNewData{false};

protected:
  size_t m_instanceID{0};
  int m_elements = 0;
  Mutex m_guard;
  bool m_hasReplacement{false};
  bool m_isInit{false};
  NO_COPY_BASE_CONSTRUCTOR(WaveTableGenerator);
};
} // namespace VeNo::Audio