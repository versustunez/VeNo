#include <JuceHeader.h>
#include <VeNo/Core/Parameter/Handler.h>

namespace VeNo::Core
{
ParameterHandler::ParameterHandler (size_t id) : m_id (id)
{
}
ParameterHandler::~ParameterHandler()
{
    for (auto& param : m_parameters)
        delete param.second;
    m_params.clear();
    m_paramMaps.clear();
    m_parameters.clear();
}

void ParameterHandler::addParameterModulate (const std::string& name, const std::string& showName, double min, double max, double value, ParameterTypes type)
{
    if (type == ParameterTypes::Boolean)
    {
        addParameter (name, showName, min, max, value, type);
        return;
    }
    m_paramMaps.push_back (name);
    auto* parameter = new ModulateParameter (name, showName, min, max, value, m_id);
    m_parameters[name] = parameter;
    m_params.push_back (parameter->createParameter (type));
}

void ParameterHandler::addParameter (const std::string& name, const std::string& showName, double min, double max, double value, ParameterTypes type)
{
    m_paramMaps.push_back (name);
    auto* parameter = new Parameter (name, showName, min, max, value, m_id);
    m_parameters[name] = parameter;
    m_params.push_back (parameter->createParameter (type));
}
juce::AudioProcessorValueTreeState::ParameterLayout ParameterHandler::setupProcessor()
{
    if (m_params.empty())
        setupParameter();
    return { m_params.begin(), m_params.end() };
}
void ParameterHandler::parameterChanged (const juce::String& parameterID, float newValue)
{
}
std::string ParameterHandler::getShowName (const std::string& parameter)
{
    if (m_parameters.contains(parameter)) {
        return m_parameters[parameter]->getShowName();
    }
    return "Unknown";
}

} // namespace VeNo::Core