#include "FXChain.h"

#include "VUtils/StringUtils.h"
#include "VeNo/Core/Instance.h"
#include "VeNo/Sound/DSP/FX/Compressor.h"
#include "VeNo/Sound/DSP/FX/Distortion.h"
#include "VeNo/Sound/DSP/FX/Filter.h"

#include <utility>

namespace VeNo::Audio {

StepLowPass::StepLowPass() {
  m_Filter.reserve(4);
  m_Filter.emplace_back();
  m_Filter.emplace_back();
  m_Filter.emplace_back();
  m_Filter.emplace_back();
}

float StepLowPass::DoFilter(float input) {
  for (auto &filter : m_Filter) {
    input = filter.processSingleSampleRaw(input);
  }
  return input;
}

void StepLowPass::SetSampleRate(double sr) {
  juce::IIRCoefficients coefficients[4]{};
  for (int j = 0; j < 2; ++j) {
    double Q =
        1.0 /
        (2.0 * std::cos((2 * j + 1) * juce::MathConstants<double>::pi / 8.0));
    auto coefficient =
        juce::IIRCoefficients::makeLowPass(sr, sr * 0.5 * 0.5, Q);
    coefficients[j * 2] = coefficient;
    coefficients[j * 2 + 1] = coefficient;
  }
  for (int i = 0; i < 4; i++)
    m_Filter[i].setCoefficients(coefficients[i]);
}

FXChain::FXChain(InstanceID id) : m_ID(id) {
  m_FX.push_back(CreateRef<Filter>(m_ID, "filter", 1)); // First Filter
  m_FX.push_back(CreateRef<Distortion>(m_ID));
  m_FX.push_back(CreateRef<Filter>(m_ID, "filter", 2)); // Second Filter
  m_FX.push_back(CreateRef<Compressor>(m_ID));
  for (size_t i = 0; i < m_FX.size(); ++i) {
    m_SortedFX.push_back(i);
  }
}

void FXChain::process(Channel &channel) {
  Channel data[2] = {{channel.left, channel.right}, {0, 0}};
  for (auto &item : m_SortedFX) {
    auto &fx = m_FX[item];
    fx->update();
  }
  for (auto &i : data) {
    i.left = m_PreFilter[0].DoFilter(i.left);
    i.right = m_PreFilter[1].DoFilter(i.right);
    for (auto &item : m_SortedFX) {
      auto &fx = m_FX[item];
      fx->process(i);
    }
    m_DCFilter.process(i);
    i.left = m_PostFilter[0].DoFilter(i.left);
    i.right = m_PostFilter[1].DoFilter(i.right);
  }

  channel.left = data[0].left * 2.0;
  channel.right = data[0].right  * 2.0;
}

void FXChain::Deserialize(const VString &data) {
  if (data.empty())
    return;
  auto split = VUtils::StringUtils::split(data, ",");
  Vector<size_t> sort;
  for (auto &item : split) {
    sort.push_back((size_t)VUtils::StringUtils::toNumber(item, 0));
  }
  if (!sort.empty()) {
    m_SortedFX = sort;
    FixMissingFX();
    Core::Instance::get(m_ID)->eventHandler.triggerEvent(
        "fx-serialized", new Events::ChangeEvent());
  }
}

VString FXChain::Serialize() {
  Vector<VString> data;
  for (auto item : m_SortedFX) {
    data.push_back(std::to_string(item));
  }
  return VUtils::StringUtils::join(data, ",");
}

void FXChain::SetSortedList(Vector<size_t> newSortedData) {
  m_SortedFX = std::move(newSortedData);
}

void FXChain::FixMissingFX() {
  if (m_FX.size() == m_SortedFX.size())
    return;
  for (size_t i = 0; i < m_FX.size(); ++i) {
    auto x = std::find(m_SortedFX.begin(), m_SortedFX.end(), i);
    if (x == m_SortedFX.end()) {
      m_SortedFX.push_back(i);
    }
  }
}
void FXChain::SetSampleRate(double sampleRate) {
  double newSR = sampleRate*2.0;
  m_DCFilter.setSampleRate(newSR);
  m_DCFilter.update();
  for (int i = 0; i < 2; ++i) {
    m_PostFilter[i].SetSampleRate(newSR);
    m_PreFilter[i].SetSampleRate(newSR);
  }
  for (auto &item : m_FX) {
    item->setSampleRate(newSR);
  }
}
} // namespace VeNo::Audio