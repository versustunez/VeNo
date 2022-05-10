#include <VeNo/Core/State/InstanceState.h>

namespace VeNo::State {
InstanceState::InstanceState(InstanceID id) : m_id(id) {
  lcdListener = CreateScope<GUI::LCDListener>(m_id);
}

InstanceState::~InstanceState() {
  if (waveEditorWindow != nullptr)
    waveEditorWindow = nullptr;
}
} // namespace VeNo::State