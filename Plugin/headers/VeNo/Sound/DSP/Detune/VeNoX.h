#pragma once

#include "DetuneStruct.h"

namespace VeNo::Audio {
class VeNoXDetune {
public:
  static void create(DetuneState & detune);
  static double getRealDetune(float detune);
};
}