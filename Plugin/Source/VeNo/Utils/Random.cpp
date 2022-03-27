#include <VeNo/Utils/Random.h>

namespace VeNo::Utils {
Random::Random() : m_mt(m_rd()) {}
double Random::generate() { return m_dist(m_mt); }
void Random::setRange(double min, double max) {
  m_dist = std::uniform_real_distribution<double>(min, max);
}
} // namespace VeNo::Utils