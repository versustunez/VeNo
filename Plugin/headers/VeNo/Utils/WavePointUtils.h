#pragma once
#include <VeNo/Sound/Tables/Structs.h>
#include <VeNo/Sound/Tables/WaveLib.h>

namespace VeNo::Utils {
struct WavePoints {
  Audio::WavePoint* point{nullptr};
  bool isCurvedPoint{false};
};
struct WavePoint {
  static WavePoints findNearest(Audio::WaveUIPoints& points, float x, float y);
  static bool updatePoint(Audio::WaveUIPoints &points, float x, float y,
                                Utils::WavePoints&, bool shiftDown, bool ctrlDown);
  static bool removeCurrentPoint(Audio::WaveUIPoints &points, Utils::WavePoints& currentPoint);
  static void finish(Audio::WaveUIPoints &points);
  static void triggerUpdate(Audio::WaveTableLib* waveLib, size_t index);
};
} // namespace VeNo::Utils