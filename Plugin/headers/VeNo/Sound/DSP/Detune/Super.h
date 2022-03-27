#pragma once

#include "DetuneStruct.h"

namespace VeNo::Audio {
class SuperDetune {
public:
  static void create(DetuneState & detune);
  static double getRealDetune(float detune);
  static double getDetuneLookup(int midi);
};
}