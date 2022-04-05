#include "VeNo/Events/EventCB.h"
#include "VeNo/Events/GUIEvents.h"

#include <VeNo/Core/Instance.h>
#include <VeNo/GUI/ActionRegistry.h>

namespace VeNo::GUI {
ActionRegistry::~ActionRegistry() {
  for (auto &action : m_actions) {
    m_handler->removeHandler(action);
  }
}

Events::EventCB *ActionRegistry::addAction(const VString &action) {
  m_actions.push_back(action);
  auto ref = CreateRef<Events::EventCB>();
  m_eventCB.push_back(ref);
  m_handler->addHandler(action, ref.get());
  return ref.get();
}

ActionRegistry::ActionRegistry(size_t instanceId) : m_id(instanceId) {
  m_handler = &Core::Instance::get(m_id)->eventHandler;
  addEditorActions();
}
void ActionRegistry::addEditorActions() {
  for (size_t i = 0; i < 6; ++i) {
    std::string actionIdx = "open_wave_editor_" + std::to_string(i + 1);
    addAction(actionIdx)->setCB([this](Events::Event *ev) {
      auto *instance = Core::Instance::get(m_id);
      auto &state = instance->state;
      if (state.waveEditorWindow)
        state.waveEditorWindow.reset(nullptr);
      auto &action = ev->as<Events::ButtonClickedEvent>()->button->action();
      auto idxString = VUtils::StringUtils::split(action, "_").back();
      auto index = (size_t)VUtils::StringUtils::toNumber(idxString, 1);
      instance->waveHolder.setCurrentTable(index - 1);
      state.waveEditorWindow =
          VeNo::CreateScope<VeNo::Windows::WaveEditorWindow>(m_id);
    });
  }
}

} // namespace VeNo::GUI