#pragma once

#include <string>

namespace VUtils
{
struct Math
{
    static double DOUBLE_PI;
    static double PI;
    static double HALF_PI;
    static double clamp (double value, double min, double max);
    static double lerp (double a, double b, double f);
    static double bezierBlend (double t);
    static double easeIn (double ratio);
    static double mixNormalized (double val1, double val2, double mix);
    static int nextPowerOfTwo (double value);
};
} // namespace VUtils