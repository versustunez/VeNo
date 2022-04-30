#include <VUtils/Math.h>
#include <VeNo/Sound/DSP/Detune/DetuneAlgos.h>
#include <VeNo/Utils/LookupTable.h>

namespace VeNo::Audio {
void SuperDetune::create(DetuneState &detune) {
  // update Detune State ;)
  detune.lookup[0] = 1.0;
  double realDetune = getRealDetune(detune.lastDetune);
  double split = realDetune / (detune.lastVoices - 1.0);
  double cents = split;
  for (int i = 1; i < detune.lastVoices; ++i) {
    double plus;
    if ((i & 1) == 1) {
      plus = VUtils::Math::centsToRatio(-cents);
    } else {
      plus = VUtils::Math::centsToRatio(cents);
    }
    cents += split;
    detune.lookup[i] = (float)plus;
  }
}
double SuperDetune::getRealDetune(float detune) {
  double d = detune / DETUNE_MAX_VALUE;
  if (d > 0.5f) {
    d = 0.5f + (d * d * 0.5f);
  }
  return d * detune;
}
} // namespace VeNo::Audio