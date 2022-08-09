#pragma once

#include "Filter.h"
#include "VeNo/Sound/DSP/FX.h"

namespace VeNo::Audio {

enum DistortionMode : int {
  OFF = 0,
  SOFT = 1,
  HARD = 2,
  RECTIFIER = 3,
  EXPERIMENTAL = 4
};
class Distortion : public FX {
public:
  explicit Distortion(InstanceID id);
  void update() override;
  void process(Channel &channel) override;

protected:
  Scope<Filter> m_Filter{nullptr};
  Core::Parameter* m_InputGain;
  Core::Parameter* m_OutputGain;
  Core::Parameter* m_Type;

  Core::ModulateParameter* m_Drive;
  Core::ModulateParameter* m_DynamicOne;
  Core::ModulateParameter* m_DynamicTwo;
  Core::ModulateParameter* m_Mix;
  int m_Mode = OFF;
};
}