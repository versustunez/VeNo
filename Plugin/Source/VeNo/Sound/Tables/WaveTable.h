#pragma once

#include <VeNo/TypeDefs.h>

namespace VeNo::Audio {

struct RawTable {
  double *Data{nullptr};
  bool IsNonImpulse{false};
  size_t Length{0};
  ~RawTable();
};

struct Wave {
  double *Data{nullptr};
  double TopFreq{0.0};
  double Length{0};
  ~Wave();
};

struct WaveTable {
  Wave *Waves;
  size_t Length;
  ~WaveTable();
};
} // namespace VeNo::Audio