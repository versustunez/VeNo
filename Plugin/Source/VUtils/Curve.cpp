#include <VUtils/Curve.h>

namespace VUtils {
CurvePoint Curve::bezierCurve(
    CurvePoint start, CurvePoint mid, CurvePoint end, double factor) {
  double xa = getPoint(start.x, mid.x, factor);
  double ya = getPoint(start.y, mid.y, factor);
  double xb = getPoint(mid.x, end.x, factor);
  double yb = getPoint(mid.y, end.y, factor);
  double x = getPoint(xa, xb, factor);
  double y = getPoint(ya, yb, factor);
  return {x, y};
}

double Curve::getPoint(double n1, double n2, double perc) {
  double diff = n2 - n1;
  return n1 + (diff * perc);
}
} // namespace VUtils
