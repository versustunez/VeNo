#include "Filter.h"

#include "VeNo/Core/Config.h"
#include "VeNo/Core/Instance.h"
#include <VUtils/LogInclude.h>

namespace VeNo::Audio {

Filter::Filter(InstanceID id) : FX(id) {
  Setup();
}


Filter::Filter(InstanceID id, const std::string &lookup_key, int index) : FX(id) {
  auto &instance = Core::Instance::get(id)->handler;
  const auto getKey = [&](const std::string& addon) {
    return index == -1 ? fmt::format("{}__{}", lookup_key, addon) : fmt::format("{}{}__{}", lookup_key, index, addon);
  };
  Frequency = instance->getModulateParameter(getKey("frequency"));
  QFactor = instance->getModulateParameter(getKey("q_factor"));
  Gain = instance->getModulateParameter(getKey("gain"));
  Type = instance->getParameter(getKey("type"));
  Setup();
}

void Filter::update() {
  // NO FILTER ;)
  auto type = Type->getInt()-1;
  if (type == FilterType::NO) {
    return;
  }
  double sampleRate = m_Config->sampleRate;
  double frequencyValue = Frequency->getValue();
  double qFactor = QFactor->getValue();
  double gain = Gain->getValue();
  if (m_SampleRate != sampleRate || m_Frequency != frequencyValue ||
      m_QFactor != qFactor || m_Gain != gain || m_Type != type) {
    // update ;)
    m_SampleRate = sampleRate;
    m_Frequency = frequencyValue;
    m_QFactor = qFactor;
    m_Gain = gain;
    juce::IIRCoefficients coefficients;
    switch (type) {
    case LP:
    case LP2:
      coefficients = juce::IIRCoefficients::makeLowPass(
          sampleRate, frequencyValue, qFactor);
      break;
    case BP:
      coefficients = juce::IIRCoefficients::makeBandPass(
          sampleRate, frequencyValue, qFactor);
      break;
    case HP:
      coefficients = juce::IIRCoefficients::makeHighPass(
          sampleRate, frequencyValue, qFactor);
      break;
    case LS:
      coefficients = juce::IIRCoefficients::makeLowShelf(
          sampleRate, frequencyValue, qFactor, (float)gain);
      break;
    case HS:
      coefficients = juce::IIRCoefficients::makeHighShelf(
          sampleRate, frequencyValue, qFactor, (float)gain);
      break;
    case NOTCH:
      coefficients = juce::IIRCoefficients::makeNotchFilter(
          sampleRate, frequencyValue, qFactor);
      break;
    case PEAK:
      coefficients = juce::IIRCoefficients::makePeakFilter(
          sampleRate, frequencyValue, qFactor, (float)gain);
      break;
    case NO:
    default:
      break;
    }
    setCoefficients(coefficients);
  }
}

void Filter::process(Channel &channel) {
  if (Type->getInt()-1 == FilterType::NO)
    return;
  channel.left = m_filters[0].processSingleSampleRaw ((float)channel.left);
  channel.right = m_filters[1].processSingleSampleRaw ((float)channel.right);
}

void Filter::Setup() {
  CheckForNullParameters();
  m_Config = &Core::Config::get();
  m_filters.emplace_back ();
  m_filters.emplace_back ();
}

void Filter::setCoefficients(juce::IIRCoefficients &coefficients) {
  for (auto& filter : m_filters)
    filter.setCoefficients (coefficients);
}

void Filter::CheckForNullParameters() {
  auto &instance = Core::Instance::get(m_ID)->handler;
  if (Type == nullptr) {
    // mean we will skip always no other parameters are needed ;)
    Type = instance->createFakeParameter(1.0);
    return;
  }
  if (Frequency == nullptr)
    Frequency = instance->createFakeModulateParameter(20000);
  if (QFactor == nullptr)
    QFactor = instance->createFakeModulateParameter(1.0);
  if (Gain == nullptr)
    Gain = instance->createFakeModulateParameter(0.0);

}

} // namespace VeNo::Audio