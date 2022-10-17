#include "FXChain.h"

#include "VUtils/StringUtils.h"
#include "VeNo/Core/Instance.h"
#include "VeNo/Sound/DSP/FX/Compressor.h"
#include "VeNo/Sound/DSP/FX/Distortion.h"
#include "VeNo/Sound/DSP/FX/Filter.h"

#include <utility>

namespace VeNo::Audio {

StepLowPass::StepLowPass() {
  for (int i = 0; i < 4; ++i) {
    filterOutput[i] = 0.0f;
    for (int j = 0; j < 16; ++j) {
      feedback[i][j] = 0.0f;
    }
  }
}

float StepLowPass::DoFilter(float input, float cut) {
  cut *= cut;
  output = input;
  if (counter >= 8)
    counter = 0;
  for (int i = 0; i < 4; ++i) {
    output = output * cut + feedback[i][counter + 7] * (1.0f - cut);
    feedback[i][counter] = output;
    feedback[i][counter + 8] = output;
    filterOutput[i] = output;
  }
  counter++;
  return filterOutput[3];
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
  Channel data[2] = {channel, {0,0}};
  for (auto &item : m_SortedFX) {
    auto &fx = m_FX[item];
    fx->update();
  }
  for (auto & i : data) {
    i.left = m_PreFilter[0].DoFilter(i.left, 1.0);
    i.right = m_PreFilter[1].DoFilter(i.right, 1.0);
    for (auto &item : m_SortedFX) {
      auto &fx = m_FX[item];
      fx->process(i);
    }
    i.left = m_PostFilter[0].DoFilter(i.left, 1.0);
    i.right = m_PostFilter[1].DoFilter(i.right, 1.0);
  }

  channel = data[0];
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
  for (auto &item : m_FX) {
    item->setSampleRate(sampleRate * 2);
  }
}
} // namespace VeNo::Audio