#pragma once
#include <VeNo/Sound/Tables/Structs.h>

namespace VeNo::Audio {
class WaveTableCreator {
public:
  static void generate(Vector<WavePoint> &points, WaveTableGroup *group);
};
} // namespace VeNo::Audio