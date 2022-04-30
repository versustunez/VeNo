#pragma once

#include "DetuneStruct.h"

#include <VeNo/Sound/Generation/OscillatorStructs.h>

namespace VeNo::Audio {
class Detune {
  enum DetuneModes { NONE, SUPER, VeNoX, UP_SHIFT, EXPERIMENTAL };

public:
  static void update(DetuneState &, OscillatorState &);
};
} // namespace VeNo::Audio