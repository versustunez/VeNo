#include <VeNo/Core/Parameter/Parameter.h>

#include <VUtils/Math.h>
#include <utility>

namespace VeNo::Core {
ModulateParameter::ModulateParameter(std::string name, std::string showName,
    double min, double max, double value, size_t id)
    : Parameter(std::move(name), std::move(showName), min, max, value, id) {
  for (double &xvalue : m_values)
    xvalue = value;

  m_preMinMax = m_max - m_min;
  m_normalised = (value - m_min) / (m_preMinMax);
}

double ModulateParameter::getVoice(int voice) {
  return voice == -1 ? m_value : m_values[voice];
}

int ModulateParameter::getVoiceInt(int voice) {
  return voice == -1 ? (int)m_value : (int)m_values[voice];
}

void ModulateParameter::setVoiceValue(int voice, double value) {
  voice == -1 ? m_value = value : m_values[voice] = value;
}

void ModulateParameter::setValue(double value) {
  m_normalised = (value - m_min) / (m_preMinMax);
  m_value = value;
  for (double &xvalue : m_values)
    xvalue = value;
}
void ModulateParameter::addValueVoice(int voice, double value) {
  m_matrix[voice] += value;
  m_values[voice] = VUtils::Math::clamp(
      m_matrix[voice] * (m_preMinMax) + m_min, m_min, m_max);
}
void ModulateParameter::addValue(double value) {
  m_matrixPos += value;
  m_value =
      VUtils::Math::clamp(m_matrixPos * (m_preMinMax) + m_min, m_min, m_max);
  for (int i = 0; i < MAX_VOICES; ++i) {
    m_matrix[i] += value;
    m_values[i] =
        VUtils::Math::clamp(m_matrix[i] * (m_preMinMax) + m_min, m_min, m_max);
  }
}
void ModulateParameter::reset() {
  m_matrixPos = m_normalised;
  for (double &value : m_matrix)
    value = m_normalised;
}
} // namespace VeNo::Core