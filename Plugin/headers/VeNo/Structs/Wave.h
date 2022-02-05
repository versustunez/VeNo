#pragma once

// Forward ;)
namespace VeNo::Audio::Waves {
class WaveTableGenerator;
}
namespace VeNo::Structs {
struct Point {
  float x = 0; // normalized (0-1)
  float y = 0; // normalized (0-1)
  float value = -500;
};
struct WavePoint {
  float x = 0; // normalized (0-1)
  float y = 0; // normalized (0-1)
  float value = 0;
  bool isNotEdge = false;
  bool curveMode = false;
  WavePoint *left = nullptr;
  WavePoint *right = nullptr;
  Point curved;
};
struct WaveUpdate {
  virtual void updated() = 0;
  VeNo::Audio::Waves::WaveTableGenerator *generator = nullptr;
};

} // namespace VeNo::Structs