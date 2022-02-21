#include <VUtils/Logging.h>
#include <VeNo/Sound/Synth/ParameterEventHandler.h>
#include <VeNo/Sound/Synth/Synthesizer.h>
#include <VeNo/Utils/ProfileMacros.h>

namespace VeNo::Audio {
void ParameterEventHandler::handle(Events::Event *) {
  VENO_PROFILE_FUNCTION();
  if (!m_synth)
    return;
  m_synth->invalidateEnvelopes();
}
} // namespace VeNo::Audio