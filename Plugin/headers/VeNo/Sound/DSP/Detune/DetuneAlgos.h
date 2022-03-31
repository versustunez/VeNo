#pragma once

#include "DetuneStruct.h"

namespace VeNo::Audio {
class SuperDetune {
public:
  static void create(DetuneState &detune, bool midiOnlyChanged);
  static double getRealDetune(float detune);
  static double getDetuneLookup(int midi);
};
class VeNoXDetune {
public:
  static void create(DetuneState & detune, bool midiOnlyChanged);
  static double getRealDetune(float detune);
};
class UPShiftDetune {
public:
  static void create(DetuneState & detune, bool midiOnlyChanged);
};
class ExperimentalDetune {
public:
  static void create(DetuneState & detune, bool midiOnlyChanged);
};
}