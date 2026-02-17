#include <VUtils/Logging.h>
#include <VeNo/Sound/Synth/ParameterEventHandler.h>
#include <VeNo/Sound/Synth/Synthesizer.h>

namespace VeNo::Audio {
void ParameterEventHandler::handle(Events::Event *event) {
  if (!m_synth)
    return;
  auto *newEvent = event->as<Events::ParameterChange>();
  if (newEvent->name == "portamento") {
    auto *voices = m_synth->voices();
    double val = m_synth->ParameterCache.Portamento->getValue() / 1000.0;
    for (int i = 0; i < MAX_VOICES; ++i)
      voices[i].midiNotePortamento.reset(m_synth->m_sampleRate, val);
  } else {
    m_synth->envelope().needRecalculate = true;
  }
}
} // namespace VeNo::Audio