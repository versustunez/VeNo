#pragma once
#include <VeNo/Sound/Tables/Structs.h>

namespace VeNo::Audio {
class WaveTable {
public:
  WaveTableGroup *m_group = new WaveTableGroup();
  Vector<WavePoint> *points{nullptr};
  size_t m_len = 0;
  void generate(Vector<WavePoint> &points);

  NO_COPY_BASE_CONSTRUCTOR(WaveTable);
};
} // namespace VeNo::Audio