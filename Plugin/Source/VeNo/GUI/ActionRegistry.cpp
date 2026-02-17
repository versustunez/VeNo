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
  addWaveFormCreatedEvents();
}

void ActionRegistry::addWaveFormCreatedEvents() {
  for (int i = 0; i < OSCILLATORS; ++i) {
    std::string actionIdx = std::format("osc{}__wave", i + 1);
    addAction(actionIdx)->setCB([&, i](Events::Event *) {
      std::string localTrigger = "waveform_" + std::to_string(i + 1);
      m_handler->triggerEvent(localTrigger, new Events::ChangeEvent);
    });
  }
  for (int i = 0; i < LFOS; ++i) {
    std::string actionIdx = std::format("lfo{}__wave", i + 1);
    int localX = i + OSCILLATORS;
    addAction(actionIdx)->setCB([&, localX](Events::Event *) {
      std::string localTrigger = "waveform_" + std::to_string(localX + 1);
      m_handler->triggerEvent(localTrigger, new Events::ChangeEvent);
    });
  }
  addAction("audioLibCreated")->setCB([&](Events::Event *) {
    for (int i = 0; i < OSCILLATORS + LFOS; ++i) {
      std::string actionIdx = "waveform_" + std::to_string(i + 1);
      m_handler->triggerEvent(actionIdx, new Events::ChangeEvent);
    }
  });
}

} // namespace VeNo::GUI