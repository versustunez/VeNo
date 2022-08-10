#include <VeNo/Core/State/InstanceState.h>

namespace VeNo::State {
InstanceState::InstanceState(InstanceID id) : m_id(id) {
  lcdListener = CreateScope<GUI::LCDListener>(m_id);
  PresetManager = CreateScope<Core::PresetManager>(m_id);
}

InstanceState::~InstanceState() = default;
void InstanceState::RegisterSynth() {
  FXChain = CreateScope<Audio::FXChain>(m_id);
  FXChain->Deserialize(PresetState["fx-series"]);
}
} // namespace VeNo::State