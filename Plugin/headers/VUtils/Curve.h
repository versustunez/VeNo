#pragma once

namespace VUtils {
struct CurvePoint {
  double x;
  double y;
};
class Curve {
public:
  static CurvePoint bezierCurve(CurvePoint start, CurvePoint mid,
                                CurvePoint end, double factor);

private:
  static double getPoint(double n1, double n2, double perc);
};
} // namespace VUtils