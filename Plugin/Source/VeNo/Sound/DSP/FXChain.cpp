#include "FXChain.h"

#include "VeNo/Sound/DSP/FX/Distortion.h"
#include "VeNo/Sound/DSP/FX/Filter.h"

namespace VeNo::Audio {

FXChain::FXChain(InstanceID id): m_ID(id) {
  m_FX.push_back(CreateRef<Distortion>(m_ID));
  m_FX.push_back(CreateRef<Filter>(m_ID));
}
void FXChain::process(Channel &channel) {
  for (auto& item : m_FX) {
    item->update();
    item->process(channel);
  }
}
} // namespace VeNo::Audio