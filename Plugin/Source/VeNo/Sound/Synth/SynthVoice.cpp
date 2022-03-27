#include <VeNo/Sound/Generation/Envelope.h>
#include <VeNo/Sound/Synth/SynthVoice.h>
#include <VeNo/Sound/Synth/Synthesizer.h>

namespace VeNo::Audio {
void SynthVoiceHelper::noteOn(Synthesizer &synth, SynthVoice &voice,
                              int channel, int note, float velocity,
                              bool legato = false) {
  voice.isActive = true;
  voice.currentChannel = channel;
  voice.currentNote = note;
  voice.legato = legato;
  voice.velocity = velocity;
  voice.isDirty = true;
  Envelope::noteOn(voice.envelopeData, synth.envelope());
}
void SynthVoiceHelper::noteOff(Synthesizer &synth, SynthVoice &voice,
                               float velocity) {
  Envelope::noteOff(voice.envelopeData, synth.envelope());
  if (voice.envelopeData.state == EnvelopeState::IDLE || velocity == 0) {
    clear(synth, voice);
  }
}
void SynthVoiceHelper::clear(Synthesizer &synth, SynthVoice &voice) {
  voice.currentNote = -1;
  voice.currentChannel = -1;
  voice.isActive = false;
  voice.velocity = 0.0;
  voice.isDirty = false;
  Envelope::noteOff(voice.envelopeData, synth.envelope());
}
} // namespace VeNo::Audio