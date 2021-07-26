#pragma once
#include "Parameter.h"
#include <JuceHeader.h>
#include <vendor/tsl/robin_map.h>

namespace VeNo::Core
{
class ParameterHandler : public juce::AudioProcessorValueTreeState::Listener
{
public:
    explicit ParameterHandler (size_t id);
    ~ParameterHandler() override;
    juce::AudioProcessorValueTreeState::ParameterLayout setupProcessor();
    void parameterChanged (const juce::String& parameterID, float newValue) override;

    std::string getShowName (const std::string& parameter);

    VeNo::Core::Parameter* getParameter (const std::string& name);
    VeNo::Core::ModulateParameter* getModulateParameter (const std::string& name);

protected:
    void setupParameter();
    void addParameterModulate (const std::string& name, const std::string& showName, double min, double max, double value, ParameterTypes type);
    void addParameter (const std::string& name, const std::string& showName, double min, double max, double value, ParameterTypes type);

    size_t m_id = 0;
    std::vector<std::string> m_paramMaps;
    tsl::robin_map<std::string, Parameter*> m_parameters;
    tsl::robin_map<std::string, ModulateParameter*> m_modularParameters;
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> m_params;
};
} // namespace VeNo::Core