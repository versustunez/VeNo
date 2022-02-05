#include <VeNo/Sound/Generation/Envelope.h>
#include <VeNo/Sound/Synth/SynthVoice.h>

namespace VeNo::Audio {
void SynthVoiceHelper::noteOn(SynthVoice &voice, int channel, int note,
    float velocity, bool legato = false) {
  voice.isActive = true;
  voice.currentChannel = channel;
  voice.currentNote = note;
  voice.legato = legato;
  voice.velocity = velocity;
  Envelope::noteOn(voice.envelopeData);
}
void SynthVoiceHelper::noteOff(SynthVoice &voice, float velocity) {
  Envelope::noteOff(voice.envelopeData);
  if (voice.envelopeData.state == EnvelopeState::IDLE || velocity == 0) {
    clear(voice);
  }
}
void SynthVoiceHelper::clear(SynthVoice &voice) {
  voice.currentNote = -1;
  voice.currentChannel = -1;
  voice.isActive = false;
  voice.velocity = 0.0;
  Envelope::noteOff(voice.envelopeData);
}
} // namespace VeNo::Audio