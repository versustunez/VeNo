#include <VUtils/Math.h>

namespace VUtils
{
double Math::DOUBLE_PI = 6.283185307179586476925286766559;
double Math::PI = 3.14159265358979323846;
double Math::HALF_PI = 1.57079632679489661923;
double Math::clamp (double value, double min, double max)
{
    return value > max ? max : value < min ? min
                                           : value;
}

double Math::lerp (double a, double b, double f) { return (a + (b - a) * f); }
double Math::bezierBlend (double t) { return t * t * (3.0 - 2.0 * t); }
double Math::easeIn (double ratio) { return ratio * ratio * ratio; }
double Math::mixNormalized (double val1, double val2, double mix)
{
    double diff = 1 - mix;
    val1 *= diff;
    val2 *= mix;
    return val1 + val2;
}
int Math::nextPowerOfTwo (double value)
{
    auto v = (unsigned int) value;
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;
    return (int) v;
}
} // namespace VUtils