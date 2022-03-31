#include <VUtils/Math.h>
#include <VeNo/Sound/DSP/Detune/DetuneAlgos.h>
#include <VeNo/Utils/LookupTable.h>

namespace VeNo::Audio {
void SuperDetune::create(DetuneState &detune, bool) {
  // update Detune State ;)
  detune.lookup[0] = 1.0;
  double realDetune = getRealDetune(detune.lastDetune);
  double split = realDetune / (detune.lastVoices - 1.0);
  double cents = split;
  double lookup = getDetuneLookup(detune.midiNote - 1);
  for (int i = 1; i < detune.lastVoices; ++i) {
    double plus;
    if ((i & 1) == 1) {
      plus = VUtils::Math::centsToRatio(-cents * lookup);
    } else {
      plus = VUtils::Math::centsToRatio(cents * lookup);
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
double SuperDetune::getDetuneLookup(int midi) {
  static Utils::LookupTable<128> lookupTable{[](float *elements, size_t size) {
    double biggest = 0.14173228346456693;
    printf("Create SuperDetune Lookup\n");
    for (size_t i = 0; i < size; ++i) {
      elements[i] = (float)(1.0 + ((int)((double)i / 7.0) / 127.0) - biggest);
    }
  }};
  return lookupTable[midi];
}
} // namespace VeNo::Audio