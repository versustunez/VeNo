#pragma once
#include <VeNo/Definitions.h>
#include <VeNo/Sound/Tables/WaveLib.h>
#include <cassert>

namespace VeNo::Audio {
struct Channel {
  double left{0};
  double right{0};
  Channel& operator*= (double factor){
    left *= factor;
    right *= factor;
    return *this;
  }
};
} // namespace VeNo::Audio