#include <VUtils/Math.h>
#include <cmath>

namespace VUtils {
double Math::DOUBLE_PI = 6.283185307179586476925286766559;
double Math::PI = 3.14159265358979323846;
double Math::HALF_PI = 1.57079632679489661923;
double Math::clamp(double value, double min, double max) {
  return value > max ? max : value < min ? min : value;
}

double Math::lerp(double a, double b, double f) { return (a + (b - a) * f); }
double Math::bezierBlend(double t) { return t * t * (3.0 - 2.0 * t); }
double Math::easeIn(double ratio) { return ratio * ratio * ratio; }
double Math::easeOut(double ratio) { return (2 * ratio * (1 - ratio) + 1.0); }
double Math::mixNormalized(double val1, double val2, double mix) {
  double diff = 1 - mix;
  val1 *= diff;
  val2 *= mix;
  return val1 + val2;
}
int Math::nextPowerOfTwo(double value) {
  auto v = (unsigned int)value;
  v--;
  v |= v >> 1;
  v |= v >> 2;
  v |= v >> 4;
  v |= v >> 8;
  v |= v >> 16;
  v++;
  return (int)v;
}
double Math::cubicInterpolate(double y0, double y1, double y2, double y3,
                              double factor) {
  double a0, a1, a2, a3, mu2;

  mu2 = factor * factor;
  a0 = y3 - y2 - y0 + y1;
  a1 = y0 - y1 - a0;
  a2 = y2 - y0;
  a3 = y1;

  return (a0 * factor * mu2 + a1 * mu2 + a2 * factor + a3);
}
double Math::map(double value, double startCoord1, double endCoord1,
                 double startCoord2, double endCoord2) {
  double offset = startCoord2;
  double ratio = (endCoord2 - startCoord2) / (endCoord1 - startCoord1);
  return ratio * (value - startCoord1) + offset;
}

double Math::centsToRatio(double cents) {
  return std::pow(2.0, cents / (double)1200);
}

double Math::gainToDb (double gain)
{
  return 20.0 * std::log10 (gain);
}


double Math::normalize(double val, float min, float max) {
  return (val - min) / (max - min);
}
} // namespace VUtils