#include "VUtils/Math.h"
#include "VeNo/Core/Config.h"

#include <VeNo/Sound/Tables/WaveTable.h>
#include <cmath>

namespace VeNo::Audio {
void WaveTable::generate(Vector<WavePoint> &inPoints) {
  auto &config = Core::Config::get();
  int maxHarms = (int)std::lround(config.sampleRate / (3.0 * 20));
  m_len = (size_t)VUtils::Math::nextPowerOfTwo(maxHarms) * 2;
  points = &inPoints;
  size_t times = 1;
  size_t last = m_len;
  while (last > 2) {
    last >>= 1u;
    times++;
  }
  m_group->len = times;
  m_group->items = new Wave[times];
  double topFreq = 2.0 / 3.0 / (double(m_len));
  size_t l = m_len;
  for (size_t i = 0; i < times; ++i) {
    auto &item = m_group->items[i];
    item.freq = topFreq;
    item.len = l;
    auto out = WaveGenerator::createArray(inPoints, l);
    item.items = out.data;
    topFreq *= 2;
    l /= 2;
  }
}
} // namespace VeNo::Audio
