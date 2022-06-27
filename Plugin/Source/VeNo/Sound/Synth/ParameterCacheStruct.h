#pragma once

#include "VeNo/Core/Parameter/Parameter.h"
#include "VeNo/Core/Parameter/Handler.h"

namespace VeNo::Audio {
struct CachedCoreParameters {
  Core::ModulateParameter *MasterVolume;
  Core::Parameter *Legato;
  Core::Parameter *Portamento;
  Core::Parameter *PitchWheel;
  Core::Parameter *AfterTouch;
  Core::Parameter *ModWheel;

  void setup(Core::ParameterHandler*);
};
}