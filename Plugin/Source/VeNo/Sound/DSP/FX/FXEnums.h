#pragma once

namespace VeNo::Audio {
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