#include "FXChain.h"

#include "VUtils/StringUtils.h"
#include "VeNo/Core/Instance.h"
#include "VeNo/Sound/DSP/FX/Distortion.h"
#include "VeNo/Sound/DSP/FX/Filter.h"

#include <utility>

namespace VeNo::Audio {

FXChain::FXChain(InstanceID id) : m_ID(id) {
  m_FX.push_back(CreateRef<Filter>(m_ID, "filter", 1)); // First Filter
  m_FX.push_back(CreateRef<Distortion>(m_ID));
  m_FX.push_back(CreateRef<Filter>(m_ID, "filter", 2)); // Second Filter
  for (size_t i = 0; i < m_FX.size(); ++i) {
    m_SortedFX.push_back(i);
  }
}

void FXChain::process(Channel &channel) {
  for (auto &item : m_SortedFX) {
    auto &fx = m_FX[item];
    fx->update();
    fx->process(channel);
  }
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
} // namespace VeNo::Audio