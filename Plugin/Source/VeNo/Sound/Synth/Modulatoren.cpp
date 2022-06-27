#include <VeNo/Core/Instance.h>
#include <VeNo/Sound/Generation/Envelope.h>
#include <VeNo/Sound/Synth/Modulatoren.h>

namespace VeNo::Audio {
void EnvModulator::init(int envelope) {
  // Create Data ;)
  m_envelope = envelope;
  m_name = fmt::format("envelope_{}", m_envelope);
  auto *instance = Core::Instance::get(m_id);
  if (instance == nullptr)
    return;
  auto &eventHandler = instance->eventHandler;
  eventHandler.addHandler(fmt::format("env{}__attack", m_envelope), this);
  eventHandler.addHandler(fmt::format("env{}__decay", m_envelope), this);
  eventHandler.addHandler(fmt::format("env{}__sustain", m_envelope), this);
  eventHandler.addHandler(fmt::format("env{}__release", m_envelope), this);
  Envelope::setup(m_data, m_id, fmt::format("env{}", m_envelope));
}
void EnvModulator::update() {
  if (m_data.needRecalculate) {
    if (Envelope::prepare(m_data)) {
      for (auto &voiceData : m_voiceData) {
        Envelope::needNextStep(voiceData, m_data);
      }
    }
  }
  for (auto &voiceData : m_voiceData) {
    Envelope::process(voiceData, m_data);
  }
}
double EnvModulator::value(int index) {
  assert(index < MAX_VOICES);
  return m_voiceData[index].value;
}
void EnvModulator::handle(Events::Event *event) {
  if (event->is<Events::ParameterChange>()) {
    m_data.needRecalculate = true;
  }
}

void EnvModulator::setSampleRate(double sampleRate) {
  m_data.sampleRate = sampleRate;
  m_data.needRecalculate = true;
}
bool EnvModulator::isVoiceModulator() { return true; }

void LFOModulator::init() {
  // Create Data ;)
  // WE will listen to changes on the EventHandler to minimize the update value
  m_name = fmt::format("lfo_{}", 1);
  auto *instance = Core::Instance::get(m_id);
  if (instance == nullptr)
    return;
  auto &eventHandler = instance->eventHandler;
}
void LFOModulator::update() {}
double LFOModulator::value(int index) { return 0.0; }
bool LFOModulator::isVoiceModulator() { return true; }

void RandomModulator::init() {
  // Create Data ;)
  m_name = fmt::format("random_{}", 1);
}
void RandomModulator::update() {}
double RandomModulator::value(int index) { return 0.0; }


void ModKnobsModulator::init() {

}

void ModKnobsModulator::update() {
}

double ModKnobsModulator::value(int index) { return 0.0; }

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
}
void ModulatorHandle::setSampleRate(double sampleRate) {
  for (auto &envelope : m_envelopes) {
    envelope->setSampleRate(sampleRate);
  }
}
void ModulatorHandle::triggerNoteOn(int voice) {
  for (auto &envelope : m_envelopes) {
    Envelope::noteOn(envelope->voiceData(voice), envelope->data());
  }
  // @TODO: Add support to trigger only LFO Voices that are on ;)
}
void ModulatorHandle::triggerNoteOff(int voice) {
  for (auto &envelope : m_envelopes) {
    Envelope::noteOff(envelope->voiceData(voice), envelope->data());
  }
}
} // namespace VeNo::Audio
