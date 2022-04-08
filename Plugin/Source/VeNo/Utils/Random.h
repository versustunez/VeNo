#pragma once

#include <random>

namespace VeNo::Utils {
class Random {
public:
  static Random &get() {
    static Random instance;
    return instance;
  }
  Random();

  double generate();
  void setRange(double min, double max);
protected:
  std::random_device m_rd{};
  std::mt19937 m_mt;
  std::uniform_real_distribution<double> m_dist{0.0, 1.0};
};
} // namespace VeNo::Utils