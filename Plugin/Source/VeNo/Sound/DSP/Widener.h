#pragma once

#include "VeNo/Sound/Structs.h"
#define SQRT2_2 0.70710678118

namespace VeNo::Audio {
struct OscillatorData;
struct WidenerState {
  double Panning[2]{SQRT2_2,SQRT2_2};
  double LastPanning{0};
  double Wideness{0};
  double Coefficient{0};
  double CoefficientM{0};
};
class Widener {
public:
  static void Update(OscillatorData&);
  static void Apply(OscillatorData&, Channel&);
  static void ApplyPanning(OscillatorData&, Channel&);
};
}