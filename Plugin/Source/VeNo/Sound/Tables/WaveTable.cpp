#include "VUtils/Math.h"
#include "VeNo/Core/Config.h"

#include <VeNo/Sound/Tables/WaveTable.h>
#include <cmath>

namespace VeNo::Audio {
void WaveTableCreator::generate(Vector<WavePoint> &inPoints, WaveTableGroup* group) {
  auto &config = Core::Config::get();
  int maxHarms = (int)std::lround(config.sampleRate / (3.0 * 20));
  auto len = (size_t)VUtils::Math::nextPowerOfTwo(maxHarms) * 2;
  size_t times = 1;
  size_t last = len;
  while (last > 2) {
    last >>= 1u;
    times++;
  }
  group->len = times;
  group->items = new Wave[times];
  double topFreq = 2.0 / 3.0 / (double(len));
  size_t l = len;
  for (size_t i = 0; i < times; ++i) {
    auto &item = group->items[i];
    item.freq = topFreq;
    item.len = l;
    auto out = WaveGenerator::createArray(inPoints, l);
    item.items = out.data;
    topFreq *= 2;
    l /= 2;
  }
}
} // namespace VeNo::Audio
