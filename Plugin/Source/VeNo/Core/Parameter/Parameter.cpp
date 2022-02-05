#include <VeNo/Core/Parameter/Parameter.h>

#include <utility>

namespace VeNo::Core {
Parameter::Parameter(std::string name, std::string showName, double min,
    double max, double value, size_t id)
    : m_id(id), m_name(std::move(name)), m_showName(std::move(showName)),
      m_max(max), m_min(min), m_value(value) {}

bool Parameter::getBool() { return m_value > 0.5; }
int Parameter::getInt() { return (int)m_value; }
double Parameter::getValue() { return m_value; }
void Parameter::setValue(double value) { m_value = value; }
Scope<juce::RangedAudioParameter> Parameter::createParameter(
    ParameterTypes type) {
  switch (type) {
  case Integer:
    return CreateScope<juce::AudioParameterInt>(
        m_name, m_showName, m_min, m_max, m_value);
  case Float:
    return CreateScope<juce::AudioParameterFloat>(
        m_name, m_showName, m_min, m_max, m_value);
  case Boolean:
    return CreateScope<juce::AudioParameterBool>(
        m_name, m_showName, m_value == 1);
  default:
    return CreateScope<juce::AudioParameterFloat>(
        m_name, m_showName, m_min, m_max, m_value);
  }
}
double Parameter::getMin() { return m_min; }
double Parameter::getMax() { return m_max; }
std::string &Parameter::getShowName() { return m_showName; }
std::string &Parameter::getName() { return m_name; }

void Parameter::parameterValueChanged(int, float newValue) {
  setValue(newValue);
}
// can be used for displays or something :D
void Parameter::parameterGestureChanged(int, bool) {}
} // namespace VeNo::Core