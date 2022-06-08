#include "VUtils/Math.h"
#include "VeNo/Core/Config.h"

#include <VeNo/Sound/Tables/WaveTable.h>
#include <cmath>

namespace VeNo::Audio {
void WaveTableCreator::generate(Vector<WavePoint> &inPoints,
                                WaveTableGroup *group) {
  auto &config = Core::Config::get();
  if (config.sampleRate < 0) {
    return;
  }
  size_t len = 2048;
  group->len = 1;
  group->items = new Wave[1];
  auto &item = group->items[0];
  item.len = len;
  auto out = WaveGenerator::createArray(inPoints, len);
  item.items = out.data;
}

float *WaveTableCreator::downSampleTable(Wave& from) {
  size_t len = from.len / 2;
  auto* output = new float[len];
  for (size_t i = 0, idx = 0; i < from.len; idx++, i+=2)
    output[idx] = from.items[i];
  return output;
}
} // namespace VeNo::Audio
