#include <VUtils/Math.h>
#include <VeNo/Sound/DSP/Detune/DetuneAlgos.h>

namespace VeNo::Audio {
void UPShiftDetune::create(DetuneState &detune) {
  detune.lookup[0] = 1.0;
  double split = detune.lastDetune / (detune.lastVoices - 1.0);
  double cents = split;
  for (int i = 1; i < detune.lastVoices; ++i) {
    detune.lookup[i] = (float)VUtils::Math::centsToRatio(cents);
    cents += split;
  }
}

void ExperimentalDetune::create(DetuneState &detune) {
  detune.lookup[0] = 1.0;
  double split = detune.lastDetune / (detune.lastVoices - 1.0);
  double cents = split * 0.75;
  for (int i = 1; i < detune.lastVoices; ++i) {
    detune.lookup[i] = (float)VUtils::Math::centsToRatio(cents);
    cents += split * 1.235464564;
    cents += (double)i / detune.lastVoices;
  }
}
} // namespace VeNo::Audio