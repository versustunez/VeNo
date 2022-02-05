#pragma once

#include "Synthesizer.h"
namespace VeNo::Audio {
class MidiHandler {
public:
  static void handleMidiMessage(
      juce::MidiMessage message, Synthesizer &synthesizer);

private:
  MidiHandler() = default;
  static void noteOn(juce::MidiMessage &message, Synthesizer &synthesizer);
  static void noteOff(juce::MidiMessage &message, Synthesizer &synthesizer);
};
} // namespace VeNo::Audio
