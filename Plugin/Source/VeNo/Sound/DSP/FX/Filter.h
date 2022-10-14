#pragma once

#include "../FX.h"
#include "FXEnums.h"
#include "VeNo/Core/Parameter/Parameter.h"

#include <JuceHeader.h>
namespace VeNo::Core {
class Config;
}
namespace VeNo::Audio {
class Filter : public FX {
public:
  explicit Filter(InstanceID id);
  explicit Filter(InstanceID id, bool dcFilter);
  explicit Filter(InstanceID id, const std::string &lookup_key, int index = -1);

  void update() override;
  void process(Channel &channel) override;

protected:
  void Setup();
  void setCoefficients(juce::IIRCoefficients &coefficients);
  void CheckForNullParameters();

public:
  Core::ModulateParameter *Frequency{};
  Core::ModulateParameter *QFactor{};
  Core::ModulateParameter *Gain{};
  Core::Parameter *Type{};

protected:
  Core::Config *m_Config{nullptr};
  double m_SampleRate{0};
  double m_Frequency{0.0};
  double m_QFactor{0.0};
  double m_Gain{0.0};
  bool m_DCFilter{false};
  FilterType m_Type{FilterType::NO};
  std::vector<juce::IIRFilter> m_filters;
};
} // namespace VeNo::Audio