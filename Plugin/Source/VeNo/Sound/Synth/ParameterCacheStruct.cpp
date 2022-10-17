#include "ParameterCacheStruct.h"

namespace VeNo::Audio {
void CachedCoreParameters::setup(Core::ParameterHandler *handler) {
  MasterVolume = handler->getModulateParameter("master__volume");
  Portamento = handler->getParameter("portamento");
  Legato = handler->getParameter("mono_legato");
  PitchWheel = handler->getParameter("pitch__wheel");
  AfterTouch = handler->getParameter("after__touch");
  ModWheel = handler->getParameter("mod__wheel");
}
} // namespace VeNo::Audio