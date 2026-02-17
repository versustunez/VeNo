#include "Widener.h"

#include "../Generation/OscillatorStructs.h"
#define PI_4 0.78539816339

namespace VeNo::Audio {
void Widener::Update(OscillatorData &oscillatorData, size_t voice) {
  auto wideness = oscillatorData.state.stereo[voice];
  auto panning = oscillatorData.state.panning[voice];
  auto &widenerState = oscillatorData.widenerState[voice];
  if (widenerState.Wideness != wideness) {
    double _width = wideness / 200.0;
    double tmp = 1 / fmax(1.0f + _width, 2.0f);
    widenerState.CoefficientM = 1 * tmp;
    widenerState.Coefficient = _width * tmp;
    widenerState.Wideness = wideness;
  }
  if (widenerState.LastPanning != panning) {
    double angle = panning * PI_4, angleCosine = std::cos(angle),
           angleSine = std::sin(angle);
    widenerState.Panning[0] = SQRT2_2 * (angleCosine - angleSine);
    widenerState.Panning[1] = SQRT2_2 * (angleCosine + angleSine);
    widenerState.LastPanning = panning;
  }
}
void Widener::Apply(WidenerState &widenerState,
                    Channel &channel) {
  double mid =
      (channel.left + channel.right) * widenerState.CoefficientM;
  double stereo =
      (channel.left - channel.right) * widenerState.Coefficient;
  channel.left = mid - stereo;
  channel.right = mid + stereo;
}

void Widener::ApplyPanning(WidenerState &widenerState,
                           Channel &channel) {
  channel.left *= widenerState.Panning[0];
  channel.right *= widenerState.Panning[1];
}
} // namespace VeNo::Audio
