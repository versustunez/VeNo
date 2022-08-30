#include <VeNo/Core/Instance.h>
#include <VeNo/Sound/Generation/Envelope.h>
#include <VeNo/Sound/Synth/Modulatoren.h>

namespace VeNo::Audio {
void ModulatorHandle::init(Matrix *matrix) {
  // we create and add Modulators here... that makes it easy to handle them ;)
  if (matrix == nullptr)
    return;

  for (int i = 2; i <= 4; ++i) {
    auto envelope = CreateRef<EnvModulator>(m_id);
    envelope->init(i);
    m_envelopes.push_back(envelope);
    matrix->addModulator(envelope);
  }

  for (int i = 1; i <= 4; ++i) {
    auto mod = CreateRef<ModKnobsModulator>(m_id);
    mod->init(i);
    m_knobsModulator.push_back(mod);
    matrix->addModulator(mod);
  }
  {
    auto mod = CreateRef<ModKnobsModulator>(m_id);
    mod->init("mod__wheel", 0);
    m_knobsModulator.push_back(mod);
    matrix->addModulator(mod);
  }

  for (int i = 1; i <= LFOS; ++i) {
    auto lfo = CreateRef<LFOModulator>(m_id);
    lfo->init(i);
    m_LFOs.push_back(lfo);
    matrix->addModulator(lfo);
  }

  for (int i = 1; i <= LFOS; ++i) {
    auto randomModulator = CreateRef<RandomModulator>(m_id);
    randomModulator->init(i);
    m_randomGenerators.push_back(randomModulator);
    matrix->addModulator(randomModulator);
  }
}
void ModulatorHandle::setSampleRate(double sampleRate) {
  for (auto &envelope : m_envelopes) {
    envelope->setSampleRate(sampleRate);
  }

  for (auto &lfo : m_LFOs) {
    lfo->m_SampleRate = sampleRate;
  }
}
void ModulatorHandle::triggerNoteOn(int voice) {
  for (auto &envelope : m_envelopes) {
    Envelope::noteOn(envelope->voiceData(voice+1), envelope->data());
    Envelope::noteOn(envelope->voiceData(0), envelope->data());
  }
}
void ModulatorHandle::triggerNoteOff(int voice) {
  for (auto &envelope : m_envelopes) {
    Envelope::noteOff(envelope->voiceData(voice+1), envelope->data());
    Envelope::noteOff(envelope->voiceData(0), envelope->data());
  }
}
} // namespace VeNo::Audio
