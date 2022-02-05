#include <VeNo/Core/Instance.h>
#include <VeNo/Core/Parameter/Handler.h>
#include <VeNo/Sound/Synth/MidiHandler.h>

namespace VeNo::Audio {
void MidiHandler::handleMidiMessage(
    juce::MidiMessage message, Synthesizer &synthesizer) {
  if (message.isNoteOn())
    return noteOn(message, synthesizer);
  if (message.isNoteOff())
    return noteOff(message, synthesizer);
  if (message.isAllNotesOff() || message.isAllSoundOff()) {
    auto *voices = synthesizer.voices();
    synthesizer.lastNoteOnCounter = 0;
    for (int i = 0; i < MAX_VOICES; i++)
      SynthVoiceHelper::noteOff(voices[i], 1.0f);
  } else if (message.isPitchWheel()) {
    double realValue = (((double)message.getPitchWheelValue() / 16383) * 2) - 1;
    synthesizer.parameterHandler()
        ->getParameter("pitch__wheel")
        ->setValue(realValue);
  } else if (message.isAftertouch()) {
    double realValue = message.getAfterTouchValue() / 127.0;
    synthesizer.parameterHandler()
        ->getParameter("after__touch")
        ->setValue(realValue);
  } else
    DBGN("Not Implemented >>  \"%s\" -> Value: %d",
        message.getControllerName(message.getControllerNumber()),
        message.getControllerValue());
}
void MidiHandler::noteOn(juce::MidiMessage &message, Synthesizer &synthesizer) {
  auto midiChannel = message.getChannel();
  auto midiNoteNumber = message.getNoteNumber();
  auto *voices = synthesizer.voices();
  for (int i = 0; i < MAX_VOICES; i++) {
    auto &voice = voices[i];
    if (voice.currentChannel == midiChannel &&
        voice.currentNote == midiNoteNumber)
      SynthVoiceHelper::noteOff(voice, 1.0);
  }
  Core::ParameterHandler &handler = *synthesizer.parameterHandler();
  bool legato = handler["mono_legato"];
  int index = 0;
  int voiceToSteal = -1;
  int highestNote = -1;
  float velocity = message.getFloatVelocity();

  for (int i = 0; i < MAX_VOICES; i++) {
    auto &voice = voices[i];
    if (!voice.isActive) {
      voice.noteOnTime = ++synthesizer.lastNoteOnCounter;
      SynthVoiceHelper::noteOn(
          voice, midiChannel, midiNoteNumber, velocity, legato);
      synthesizer.hasActiveNote = true;
      voiceToSteal = -1;
      break;
    }

    if (legato) {
      voiceToSteal = index;
      synthesizer.lastNoteOnCounter = 0;
      synthesizer.hasActiveNote = true;
      break;
    }
    if (voice.currentNote > highestNote &&
        (voiceToSteal == -1 ||
            voice.noteOnTime < voices[voiceToSteal].noteOnTime))
      voiceToSteal = index;
    index++;
  }
  if (voiceToSteal != -1) {
    auto &voice = voices[voiceToSteal];
    if (!legato) {
      SynthVoiceHelper::noteOff(voice, 0.0f);
      voice.noteOnTime = ++synthesizer.lastNoteOnCounter;
    }
    SynthVoiceHelper::noteOn(
        voice, midiChannel, midiNoteNumber, velocity, legato);
    synthesizer.hasActiveNote = true;
  }
}
void MidiHandler::noteOff(
    juce::MidiMessage &message, Synthesizer &synthesizer) {
  synthesizer.hasActiveNote = false;
  auto *voices = synthesizer.voices();
  auto midiChannel = message.getChannel();
  auto midiNoteNumber = message.getNoteNumber();
  for (int i = 0; i < MAX_VOICES; i++) {
    auto &voice = voices[i];
    if (voice.currentNote == midiNoteNumber &&
        voice.currentChannel == midiChannel)
      SynthVoiceHelper::noteOff(voice, voice.velocity);
    else if (voice.isActive)
      synthesizer.hasActiveNote = true;
  }
}
} // namespace VeNo::Audio
