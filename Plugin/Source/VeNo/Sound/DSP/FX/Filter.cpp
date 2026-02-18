#include "Filter.h"

#include "VUtils/LogInclude.h"
#include "VeNo/Core/Config.h"
#include "VeNo/Core/Instance.h"

namespace VeNo::Audio {

Filter::Filter(InstanceID id) : FX(id) { Setup(); }

Filter::Filter(InstanceID id, bool isDcFilter)
    : FX(id),
      m_DCFilter(isDcFilter) {
  Setup();
}

Filter::Filter(InstanceID id, const std::string &lookup_key, int index)
    : FX(id) {
  auto &instance = Core::Instance::get(id)->handler;
  const auto getKey = [&](const std::string &addon) {
    return index == -1 ? fmt::format("{}__{}", lookup_key, addon)
                       : fmt::format("{}{}__{}", lookup_key, index, addon);
  };

  if (index != -1)
    SetName(fmt::format("Filter {}", index));
  Frequency = instance->getModulateParameter(getKey("frequency"));
  QFactor = instance->getModulateParameter(getKey("q_factor"));
  Gain = instance->getModulateParameter(getKey("gain"));
  Type = instance->getParameter(getKey("type"));
  Setup();
}

void Filter::update() {
  // NO FILTER ;)
  double sampleRate =
      m_FXSampleRate == 0 ? m_Config->sampleRate : m_FXSampleRate;
  if (m_DCFilter) {
    m_Filter.SetFilterType(FilterImpl::Type::HighPass);
    m_Filter.CalculateCoefficients(0, 10.0, 0.7);
    return;
  }
  bool typeChanged = m_Filter.SetFilterTypeByNumber(Type->getInt() - 1);
  if (Type->getInt() == 0) {
    return;
  }

  double frequencyValue = Frequency->getValue();
  double qFactor = QFactor->getValue();
  double gain = Gain->getValue();
  if (m_SampleRate != sampleRate || m_Frequency != frequencyValue ||
      m_QFactor != qFactor || m_Gain != gain || typeChanged) {
    // update ;)
    m_SampleRate = sampleRate;
    m_Frequency = frequencyValue;
    m_QFactor = qFactor;
    m_Gain = gain;
    m_Filter.SetSampleRate(m_SampleRate);
    m_Filter.CalculateCoefficients(m_Gain, m_Frequency, m_QFactor);
  }
}

void Filter::process(Channel &channel) {
  if (m_Filter.IsBypassed())
    return;
  channel.left = m_Filter.ApplyLeft(channel.left);
  channel.right = m_Filter.ApplyRight(channel.right);
}

void Filter::Setup() {
  if (!m_DCFilter) {
    CheckForNullParameters();
  }
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
