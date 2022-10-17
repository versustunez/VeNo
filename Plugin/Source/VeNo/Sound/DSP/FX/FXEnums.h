#pragma once

namespace VeNo::Audio {
enum class FilterType : int {
  NO = 0,
  LP = 1,
  LP2 = 2, // FIRST ORDER LP! NO JUCE :P
  BP = 3,
  HP = 4,
  NOTCH = 5,
  LS = 6,
  HS = 7,
  PEAK = 8,
  END
};

enum class DistortionMode : int {
  OFF,
  SOFT,
  HARD,
  RECTIFIER,
  HALF_RECTIFIER,
  SIN,
  TAN,
  FOLD_BACK,
  END
};
} // namespace VeNo::Audio