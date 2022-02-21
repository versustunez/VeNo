#pragma once
#include "Parameter.h"

#include <JuceHeader.h>

namespace VeNo::Core {
class ParameterHandler : public juce::AudioProcessorValueTreeState::Listener {
public:
  explicit ParameterHandler(size_t id);
  ~ParameterHandler() override;
  juce::AudioProcessorValueTreeState::ParameterLayout setupProcessor();
  void parameterChanged(const juce::String &parameterID,
                        float newValue) override;

  std::string getShowName(const std::string &parameter);

  Parameter *getParameter(const std::string &name);
  Parameter *getParameter(const std::string &prefix,
                          const std::string &parameter);
  Parameter *getOscParameter(const std::string &name, int id);
  ModulateParameter *getModulateParameter(const std::string &name);

  Parameter *operator[](const char *name);

  bool hasParameter(const char *name);

protected:
  void setupParameter();
  void addParameterModulate(const std::string &name,
                            const std::string &showName, double min, double max,
                            double value, ParameterTypes type);
  void addParameter(const std::string &name, const std::string &showName,
                    double min, double max, double value, ParameterTypes type);

  size_t m_id = 0;
  std::vector<std::string> m_paramMaps;
  std::unordered_map<std::string, Parameter *> m_parameters;
  std::unordered_map<std::string, ModulateParameter *> m_modularParameters;
  std::vector<Scope<juce::RangedAudioParameter>> m_params;
};
} // namespace VeNo::Core
