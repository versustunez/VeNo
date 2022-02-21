#pragma once
#include <VeNo/Definitions.h>
#include <VeNo/Sound/Tables/TableGenerator.h>

namespace VeNo::Audio {
struct Channel {
  double left;
  double right;
};

struct WaveHolder {
  explicit WaveHolder(size_t id);
  Scope<WaveTableGenerator> generators[OSCILLATORS];
};
} // namespace VeNo::Audio