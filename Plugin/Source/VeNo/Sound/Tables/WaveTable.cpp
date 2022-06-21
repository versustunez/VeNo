#include "WaveTable.h"

namespace VeNo::Audio {

RawTable::~RawTable() {
  delete[] Data;
}

Wave::~Wave() {
  delete[] Data;
}

WaveTable::~WaveTable() {
  delete[] Waves;
}
}