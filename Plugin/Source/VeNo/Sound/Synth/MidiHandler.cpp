#include <VeNo/Core/Instance.h>
#include <VeNo/Core/Parameter/Handler.h>
#include <VeNo/Sound/Synth/MidiHandler.h>

namespace VeNo::Audio {
void MidiHandler::handleMidiMessage(juce::MidiMessage message,
                                    Synthesizer &synthesizer) {
  if (message.isNoteOn())
    return noteOn(message, synthesizer);
  if (message.isNoteOff())
    return noteOff(message, synthesizer);
  if (message.isAllNotesOff() || message.isAllSoundOff()) {
    auto *voices = synthesizer.voices();
    synthesizer.lastNoteOnCounter = 0;
    for (int i = 0; i < MAX_VOICES; i++) {
      synthesizer.matrix().handle().triggerNoteOff(i);
      SynthVoiceHelper::noteOff(synthesizer, *voices[i], 1.0f);
    }
  } else if (message.isPitchWheel()) {
    double realValue = (((double)message.getPitchWheelValue() / 16383) * 2) - 1;
    synthesizer.ParameterCache.PitchWheel->setValue(realValue);
  } else if (message.isAftertouch()) {
    double realValue = message.getAfterTouchValue() / 127.0;
    synthesizer.ParameterCache.AfterTouch->setValue(realValue);
  } else if (message.isController()) {
    if (message.getControllerNumber() == 1) {
      synthesizer.ParameterCache.ModWheel->setValue(
          message.getControllerValue() / 127.0);
    }
  }
}
void MidiHandler::noteOn(juce::MidiMessage &message, Synthesizer &synthesizer) {
  auto midiChannel = message.getChannel();
  auto midiNoteNumber = message.getNoteNumber();
  auto *voices = synthesizer.voices();
  for (int i = 0; i < MAX_VOICES; i++) {
    auto &voice = voices[i];
    if (voice->isActive && voice->currentChannel == midiChannel &&
        voice->currentNote == midiNoteNumber) {
      synthesizer.matrix().handle().triggerNoteOff(i);
      SynthVoiceHelper::noteOff(synthesizer, *voice, 1.0);
    }
  }
  bool legato = synthesizer.ParameterCache.Legato->getBool();
  int index = 0;
  int voiceToSteal = -1;
  float velocity = message.getFloatVelocity();

  for (int i = 0; i < MAX_VOICES; i++) {
    auto &voice = voices[i];
    if (!voice->isActive || voice->envelopeData.state == EnvelopeState::IDLE) {
      voice->noteOnTime = ++synthesizer.lastNoteOnCounter;
      SynthVoiceHelper::noteOn(synthesizer, *voice, midiChannel, midiNoteNumber,
                               velocity, legato);
      synthesizer.matrix().handle().triggerNoteOn(i);
      synthesizer.hasActiveNote = true;
      return;
    }

    if (legato) {
      voiceToSteal = index;
      synthesizer.lastNoteOnCounter = 0;
      synthesizer.hasActiveNote = true;
      break;
    }
    // Steps:
    // Select first if not selected any note
    if (voiceToSteal == -1 ||
        (voice->envelopeData.state == EnvelopeState::RELEASE &&
         voice->noteOnTime < voices[voiceToSteal]->noteOnTime))
      voiceToSteal = index;
    index++;
  }
  if (voiceToSteal != -1) {
    auto &voice = voices[voiceToSteal];
    if (!legato) {
      SynthVoiceHelper::noteOff(synthesizer, *voice, voice->velocity);
      voice->noteOnTime = ++synthesizer.lastNoteOnCounter;
      synthesizer.matrix().handle().triggerNoteOn(voiceToSteal);
    }
    SynthVoiceHelper::noteOn(synthesizer, *voice, midiChannel, midiNoteNumber,
                             velocity, legato);
    synthesizer.hasActiveNote = true;
  }
}
void MidiHandler::noteOff(juce::MidiMessage &message,
                          Synthesizer &synthesizer) {
  synthesizer.hasActiveNote = false;
  auto *voices = synthesizer.voices();
  auto midiChannel = message.getChannel();
  auto midiNoteNumber = message.getNoteNumber();
  for (int i = 0; i < MAX_VOICES; i++) {
    auto &voice = voices[i];
    if (voice->isActive && voice->currentNote == midiNoteNumber &&
        voice->currentChannel == midiChannel) {
      synthesizer.matrix().handle().triggerNoteOff(i);
      SynthVoiceHelper::noteOff(synthesizer, *voice, voice->velocity);
      if (voice->envelopeData.state != EnvelopeState::IDLE) {
        synthesizer.hasActiveNote = true;
      }
    } else if (voice->isActive)
      synthesizer.hasActiveNote = true;
  }
}
} // namespace VeNo::Audio
