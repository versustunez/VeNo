#include "VeNo/Core/Instance.h"

#include <VeNo/Sound/Generation/Envelope.h>

namespace VeNo::Audio {
void Envelope::setup(EnvelopeData &data, size_t instanceId,
                     const std::string &envelope) {
  auto *handler = Core::Instance::get(instanceId)->handler.get();
  RawEnvelope &rawData = data.parameters;
  rawData.attack = handler->getParameter(envelope, "attack");
  rawData.decay = handler->getParameter(envelope, "decay");
  rawData.sustain = handler->getParameter(envelope, "sustain");
  rawData.release = handler->getParameter(envelope, "release");
}

bool Envelope::prepare(EnvelopeData &data) {
  if (data.needRecalculate) {
    data.sustain = data.parameters.sustain->getValue();
    data.attack = getRate(1.0, data.parameters.attack->getValue() / 1000,
                          data.sampleRate);
    data.decay =
        getRate(1.0, data.parameters.decay->getValue() / 1000, data.sampleRate);
    data.release =
        getRate(data.sustain, data.parameters.release->getValue() / 1000,
                data.sampleRate);
    data.needRecalculate = false;
    return true;
  }
  return false;
}

double Envelope::process(EnvelopeVoiceData &voiceData, EnvelopeData &envelope) {
  switch (voiceData.state) {
  case IDLE: voiceData.value = 0.0; break;
  case ATTACK:
    voiceData.value += envelope.attack;
    if (voiceData.value >= 1.0) {
      voiceData.value = 1.0;
      nextStep(voiceData, envelope);
    }
    break;
  case DECAY:
    voiceData.value -= envelope.decay;
    if (voiceData.value <= envelope.sustain) {
      voiceData.value = envelope.sustain;
      nextStep(voiceData, envelope);
    }
    break;
  case SUSTAIN: voiceData.value = envelope.sustain; break;
  case RELEASE:
    voiceData.value -= voiceData.releaseRate;
    if (voiceData.value <= 0.0f)
      nextStep(voiceData, envelope);
  }
  return voiceData.value;
}

void Envelope::needNextStep(EnvelopeVoiceData &voice, EnvelopeData &envelope) {
  voice.releaseRate = envelope.release;
  auto state = voice.state;
  if ((state == EnvelopeState::ATTACK && envelope.attack <= 0.0f) ||
      (state == EnvelopeState::DECAY &&
       (envelope.decay <= 0.0f || voice.value <= envelope.sustain)) ||
      (state == EnvelopeState::RELEASE && envelope.release <= 0.0f)) {
    nextStep(voice, envelope);
  }
}

void Envelope::noteOff(EnvelopeVoiceData &data, EnvelopeData &envelope) {
  if (data.state != IDLE) {
    double release = envelope.parameters.release->getValue() / 1000;
    if (release > 0.0) {
      data.releaseRate = getRate(data.value, release, envelope.sampleRate);
      data.state = RELEASE;
    } else {
      data.value = 0.0;
      data.state = EnvelopeState::IDLE;
    }
  }
}

void Envelope::noteOn(EnvelopeVoiceData &data, EnvelopeData &envelope) {
  data.releaseRate = envelope.release;
  if (envelope.attack > 0.0) {
    data.state = EnvelopeState::ATTACK;
  } else if (envelope.decay > 0.0) {
    data.value = 1.0;
    data.state = EnvelopeState::DECAY;
  } else {
    data.value = envelope.sustain;
    data.state = EnvelopeState::SUSTAIN;
  }
}

void Envelope::nextStep(EnvelopeVoiceData &voiceData, EnvelopeData &data) {
  if (voiceData.state == EnvelopeState::ATTACK)
    voiceData.state =
        (data.decay > 0.0f ? EnvelopeState::DECAY : EnvelopeState::SUSTAIN);
  else if (voiceData.state == EnvelopeState::DECAY)
    voiceData.state = EnvelopeState::SUSTAIN;
  else if (voiceData.state == EnvelopeState::RELEASE) {
    voiceData.value = 0.0;
    voiceData.state = EnvelopeState::IDLE;
  }
}

} // namespace VeNo::Audio