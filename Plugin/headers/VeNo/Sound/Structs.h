#pragma once
#define MAX_OSCILLATORS 4
#define MAX_UNISON_VOICES 9
namespace VeNo::Audio {
struct Channel {
  double left;
  double right;
};
} // namespace VeNo::Audio