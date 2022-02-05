#pragma once

#include "EnvelopeStructs.h"
namespace VeNo::Audio {
class Envelope {
public:
  static void noteOff(EnvelopeVoiceData &);
  static void noteOn(EnvelopeVoiceData &);

protected:
};
} // namespace VeNo::Audio