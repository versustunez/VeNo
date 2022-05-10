#pragma once

#include "DetuneStruct.h"

namespace VeNo::Audio {
class SuperDetune {
public:
  static void create(DetuneState &detune);
  static double getRealDetune(float detune);
};
class VeNoXDetune {
public:
  static void create(DetuneState &detune);
  static double getRealDetune(float detune);
};
class UPShiftDetune {
public:
  static void create(DetuneState &detune);
};
class ExperimentalDetune {
public:
  static void create(DetuneState &detune);
};
} // namespace VeNo::Audio