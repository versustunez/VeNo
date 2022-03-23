#pragma once
#include <VeNo/Definitions.h>
#include <VeNo/Sound/Tables/WaveLib.h>

namespace VeNo::Audio {
struct Channel {
  double left;
  double right;
};

struct WaveHolder {
  explicit WaveHolder(size_t id);
  Scope<WaveTableLib> generators[OSCILLATORS];
};
} // namespace VeNo::Audio