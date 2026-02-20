#pragma once

#include <VeNo/TypeDefs.h>
#include <array>

namespace VeNo::Audio {

struct RawTable {
  static constexpr size_t TableLength = 2048;
  std::array<double, TableLength> Data{};
  bool IsNonImpulse{false};
};

struct Wave {
  static constexpr size_t TableLength = RawTable::TableLength;
  std::array<float, RawTable::TableLength+1> Data{};
  double TopFreq{0.0};
};

struct WaveTable {
  Wave *Waves;
  size_t Length;
  ~WaveTable();
};
} // namespace VeNo::Audio