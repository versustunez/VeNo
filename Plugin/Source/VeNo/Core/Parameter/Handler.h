#pragma once
#include "Parameter.h"

#include <JuceHeader.h>

namespace VeNo::Core {
class ParameterHandler : public juce::AudioProcessorValueTreeState::Listener {
public:
  explicit ParameterHandler(InstanceID id);
  ~ParameterHandler() override;
  juce::AudioProcessorValueTreeState::ParameterLayout setupProcessor();
  void parameterChanged(const juce::String &parameterID,
                        float newValue) override;

  std::string getShowName(const std::string &parameter);

  Parameter *getParameter(const std::string &name);
  Parameter *getParameter(const std::string &prefix,
                          const std::string &parameter);
  Parameter *getOscParameter(const std::string &name, int id);
  ModulateParameter *getOscModulateParameter(const std::string &name, int id);
  ModulateParameter *getModulateParameter(const std::string &name);

  Parameter *operator[](const char *name);

  bool hasParameter(const char *name);
  Parameter *createFakeParameter(double value);
  ModulateParameter *createFakeModulateParameter(double value);

protected:
  void setupParameter();
  void addParameterModulate(const std::string &name,
                            const std::string &showName, double min, double max,
                            double value, ParameterTypes type);
  void addParameter(const std::string &name, const std::string &showName,
                    double min, double max, double value, ParameterTypes type);

  InstanceID m_id = 0;
  Vector<std::string> m_paramMaps;
  Map<std::string, Parameter *> m_parameters;
  Map<std::string, ModulateParameter *> m_modularParameters;
  Vector<Scope<juce::RangedAudioParameter>> m_params;
  Vector<Parameter *> m_FakeParameter{};
};
} // namespace VeNo::Core
