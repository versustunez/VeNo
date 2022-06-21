#pragma once

#include "VeNo/Definitions.h"
#include "WaveTable.h"

#include <VeNo/TypeDefs.h>

namespace VeNo::Audio {

class WaveLib {
public:
  ~WaveLib();
  static WaveLib &Get();
  WaveTable* GetWaveTable(int index);
  void SetWaveTable(int index, WaveTable* table);
  void CreateAll();
protected:
  void Clear();
protected:
  static Mutex createGuard;
  WaveTable* m_WaveTables[MAX_TABLES]{};
  double m_SampleRate{0};
};
}