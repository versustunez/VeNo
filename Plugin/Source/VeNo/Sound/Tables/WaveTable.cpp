#include "WaveTable.h"

namespace VeNo::Audio {

WaveTable::~WaveTable() { delete[] Waves; }
} // namespace VeNo::Audio