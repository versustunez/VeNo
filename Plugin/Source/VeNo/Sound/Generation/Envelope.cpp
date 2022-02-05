#include <VeNo/Sound/Generation/Envelope.h>

namespace VeNo::Audio {
void Envelope::noteOff(EnvelopeVoiceData &data) {
  if (data.state != IDLE)
    data.state = RELEASE;
}
void Envelope::noteOn(EnvelopeVoiceData &data) {
  data.state = ATTACK;
  data.value = 0;
}
} // namespace VeNo::Audio