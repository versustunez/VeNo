#pragma once

#include <VeNo/Events/Event.h>

namespace VeNo::Audio {
class Synthesizer;
class ParameterEventHandler : public Events::Handler {
public:
  void handle(Events::Event* event) override;
  void setSynthesizer(Synthesizer *synthesizer) { m_synth = synthesizer; };

private:
  Synthesizer *m_synth{nullptr};
};
} // namespace VeNo::Audio