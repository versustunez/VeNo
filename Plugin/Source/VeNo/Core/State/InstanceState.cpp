#include <VeNo/Core/State/InstanceState.h>

namespace VeNo::State {
InstanceState::InstanceState(size_t id) : m_id(id) {}
InstanceState::~InstanceState() {
  if (waveEditorWindow != nullptr)
    waveEditorWindow = nullptr;
}
} // namespace VeNo::State