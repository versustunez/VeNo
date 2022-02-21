#pragma once
#include "EnvelopeStructs.h"

#include <cstddef>

namespace VeNo::Audio {
class Envelope {
public:
  static void setup(EnvelopeData &data, size_t instanceId,
                    const std::string &envelope);
  static bool prepare(EnvelopeData &data);
  static double process(EnvelopeVoiceData &, EnvelopeData &);
  static void nextStep(EnvelopeVoiceData &, EnvelopeData &);
  static void needNextStep(EnvelopeVoiceData &, EnvelopeData &);
  static void noteOff(EnvelopeVoiceData &, EnvelopeData &);
  static void noteOn(EnvelopeVoiceData &, EnvelopeData &);

protected:
  static double getRate(double distance, double timeInSeconds, double sr) {
    return timeInSeconds > 0.0 ? (distance / (timeInSeconds * sr)) : -1.0;
  }
};
} // namespace VeNo::Audio