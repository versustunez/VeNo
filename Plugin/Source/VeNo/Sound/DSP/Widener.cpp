#include "Widener.h"

#include "../Generation/OscillatorStructs.h"
#define PI_4 0.78539816339

namespace VeNo::Audio {
void Widener::Update(VeNo::Audio::OscillatorData & oscillatorData) {
  auto wideness = oscillatorData.state.stereo->getVoice(oscillatorData.id);
  auto panning = oscillatorData.state.panning->getVoice(oscillatorData.id);
  auto& widenerState = oscillatorData.widenerState;
  if (widenerState.Wideness != wideness) {
    double _width = wideness / 200.0;
    double tmp = 1 / fmax (1.0f + _width, 2.0f);
    widenerState.CoefficientM = 1 * tmp;
    widenerState.Coefficient = _width * tmp;
    widenerState.Wideness = wideness;
  }
  if (widenerState.LastPanning != panning) {
    double angle = panning * PI_4;
    widenerState.Panning[0] = SQRT2_2 * double (std::cos (angle) - std::sin (angle));
    widenerState.Panning[1] = SQRT2_2 * double (std::cos (angle) + std::sin (angle));
    widenerState.LastPanning = panning;
  }
}
void Widener::Apply(VeNo::Audio::OscillatorData & oscillatorData, Channel& channel) {
  double mid = (channel.left + channel.right) * oscillatorData.widenerState.CoefficientM;
  double stereo = (channel.left - channel.right) * oscillatorData.widenerState.Coefficient;
  channel.left = mid - stereo;
  channel.right = mid + stereo;
}

void Widener::ApplyPanning(VeNo::Audio::OscillatorData& oscillatorData, Channel& channel) {
  channel.left *= oscillatorData.widenerState.Panning[0];
  channel.right *= oscillatorData.widenerState.Panning[1];
}
} // namespace VeNo::Audio
