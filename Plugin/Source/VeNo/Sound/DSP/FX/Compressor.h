#pragma once

#include "VeNo/Core/Parameter/Parameter.h"
#include "VeNo/Sound/DSP/FX.h"
namespace VeNo::Audio {
class Compressor : public FX {
public:
  explicit Compressor(InstanceID id);
  void update() override;
  void process(Channel &channel) override;
  void setSampleRate(double d) override;
protected:
  void CalculateAttack();
  void CalculateRelease();
protected:
  Core::Parameter* m_IsActiveParameter;
  Core::Parameter* m_ThreshHoldParameter;
  Core::Parameter* m_RatioParameter;
  Core::Parameter* m_ReleaseParameter;
  Core::Parameter* m_AttackParameter;

  double m_AttackMS{0};
  double m_ReleaseMS{0};
  double m_Threshold{0};
  double m_ThresholdGain{0};

  double m_AttackCT{0};
  double m_ReleaseCT{0};
  double m_SampleRate{44100};

  double m_State;
};
}

