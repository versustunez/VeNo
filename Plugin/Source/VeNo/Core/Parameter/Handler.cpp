#include "VUtils/Logging.h"
#include "VeNo/Core/Parameter/Parameter.h"

#include <JuceHeader.h>
#include <VeNo/Core/Parameter/Handler.h>

namespace VeNo::Core {
ParameterHandler::ParameterHandler(size_t id) : m_id(id) {}
ParameterHandler::~ParameterHandler() {
  for (auto &param : m_parameters)
    delete param.second;
  m_params.clear();
  m_paramMaps.clear();
  m_parameters.clear();
}

void ParameterHandler::addParameterModulate(const std::string &name,
                                            const std::string &showName,
                                            double min, double max,
                                            double value, ParameterTypes type) {
  if (type == ParameterTypes::Boolean) {
    addParameter(name, showName, min, max, value, type);
    return;
  }
  m_paramMaps.push_back(name);
  auto *parameter =
      new ModulateParameter(name, showName, min, max, value, m_id);
  m_parameters[name] = parameter;
  m_modularParameters[name] = parameter;
  m_params.push_back(parameter->createParameter(type));
  auto *back = m_params.back().get();
  back->addListener(parameter);
  parameter->calculatePrecision(back->getNormalisableRange().interval);
}

void ParameterHandler::addParameter(const std::string &name,
                                    const std::string &showName, double min,
                                    double max, double value,
                                    ParameterTypes type) {
  m_paramMaps.push_back(name);
  auto *parameter = new Parameter(name, showName, min, max, value, m_id);
  m_parameters[name] = parameter;
  m_params.push_back(parameter->createParameter(type));
  m_params.back()->addListener(parameter);
}
juce::AudioProcessorValueTreeState::ParameterLayout
ParameterHandler::setupProcessor() {
  if (m_params.empty())
    setupParameter();
  return {m_params.begin(), m_params.end()};
}
void ParameterHandler::parameterChanged(const juce::String &, float) {}
std::string ParameterHandler::getShowName(const std::string &parameter) {
  if (m_parameters.find(parameter) != m_parameters.end())
    return m_parameters[parameter]->getShowName();
  return "Unknown";
}
Core::Parameter *ParameterHandler::getParameter(const std::string &name) {
  if (m_parameters.find(name) != m_parameters.end())
    return m_parameters[name];
  return nullptr;
}

Core::Parameter *ParameterHandler::getParameter(const std::string &prefix,
                                                const std::string &parameter) {
  std::string name = prefix + "__" + parameter;
  if (m_parameters.find(name) != m_parameters.end())
    return m_parameters[name];
  return nullptr;
}
Core::Parameter *ParameterHandler::getOscParameter(const std::string &name,
                                                   int id) {
  std::string parameter = "osc" + std::to_string(id) + "__" + name;
  return getParameter(parameter);
}
Core::ModulateParameter *
ParameterHandler::getModulateParameter(const std::string &name) {
  if (m_modularParameters.find(name) != m_modularParameters.end())
    return m_modularParameters[name];
  return nullptr;
}
Parameter *ParameterHandler::operator[](const char *name) {
  if (m_parameters.find(name) != m_parameters.end())
    return m_parameters[name];
  return nullptr;
}
bool ParameterHandler::hasParameter(const char *name) {
  return m_parameters.find(name) != m_parameters.end();
}

} // namespace VeNo::Core
