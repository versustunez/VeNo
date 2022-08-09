#pragma once

#include "../FX.h"
#include "VeNo/Core/Parameter/Parameter.h"

#include <JuceHeader.h>
namespace VeNo::Core {
class Config;
}
namespace VeNo::Audio {
enum FilterType {
  NO = 0,
  LP = 1,
  LP2 = 2, // FIRST ORDER LP! NO JUCE :P
  BP = 3,
  HP = 4,
  NOTCH = 5,
  LS = 6,
  HS = 7,
  PEAK = 8
};
class Filter : public FX {
public:
  explicit Filter(InstanceID id);
  explicit Filter(InstanceID id, const std::string& lookup_key, int index = -1);

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
  Core::Config* m_Config{ nullptr};
  double m_SampleRate{0};
  double m_Frequency{0.0};
  double m_QFactor{0.0};
  double m_Gain{0.0};
  FilterType m_Type{FilterType::NO};
  std::vector<juce::IIRFilter> m_filters;
};
} // namespace VeNo::Audio