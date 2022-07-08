#include "Filter.h"

#include "VeNo/Core/Config.h"
#include "VeNo/Core/Instance.h"

namespace VeNo::Audio {

Filter::Filter(InstanceID id) : FX(id) {
  auto &instance = Core::Instance::get(id)->handler;
  Type = instance->createFakeParameter(0);
  m_Config = &Core::Config::get();
  m_filters.emplace_back ();
  m_filters.emplace_back ();
}

void Filter::update() {
  // NO FILTER ;)
  auto type = (FilterType)Type->getInt();
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
    case NO: break;
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
    }
    setCoefficients(coefficients);
  }
}

void Filter::process(Channel &channel) {
  if (Type->getInt() == FilterType::NO)
    return;
  channel.left = m_filters[0].processSingleSampleRaw (channel.left);
  channel.right = m_filters[1].processSingleSampleRaw (channel.right);
}

void Filter::setCoefficients(juce::IIRCoefficients &coefficients) {
  for (auto& filter : m_filters)
    filter.setCoefficients (coefficients);
}

} // namespace VeNo::Audio