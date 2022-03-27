#pragma once

#include "DetuneStruct.h"

#include <VeNo/Sound/Generation/OscillatorStructs.h>

namespace VeNo::Audio {
class Detune {
  enum DetuneModes { NONE, SUPER, VeNoX };

public:
  static void update(DetuneState &, OscillatorState &, int midi);
};
} // namespace VeNo::Audio