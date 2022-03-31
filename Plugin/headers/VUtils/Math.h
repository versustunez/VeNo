#pragma once

#include <string>

namespace VUtils {
struct Math {
  static double DOUBLE_PI;
  static double PI;
  static double HALF_PI;
  static double clamp(double value, double min, double max);
  static double lerp(double a, double b, double f);
  static double bezierBlend(double t);
  static double easeIn(double ratio);
  static double easeOut(double ratio);
  static double cubicInterpolate(double y0, double y1, double y2, double y3,
                                 double factor);
  static double mixNormalized(double val1, double val2, double mix);
  static double map(double value, double startCoord1, double endCoord1,
                    double startCoord2, double endCoord2);
  static int nextPowerOfTwo(double value);
  static double centsToRatio(double cents);
  static double normalize(double val, float min, float max);
};
} // namespace VUtils