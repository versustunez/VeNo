#include "../Modulatoren.h"

#include <VeNo/Core/Instance.h>
#include <VeNo/Sound/Generation/Envelope.h>
namespace VeNo::Audio {
void EnvModulator::init(int envelope) {
  // Create Data ;)
  m_envelope = envelope;
  m_name = fmt::format("Envelope {}", m_envelope);
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
  return index == -1 ? m_voiceData[0].value : m_voiceData[index+1].value;
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
} // namespace VeNo::Audio