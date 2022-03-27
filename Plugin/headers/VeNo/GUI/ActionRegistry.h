#pragma once

#include <VeNo/Events/Event.h>
#include <VeNo/Events/EventCB.h>
#include <VeNo/Events/EventHandler.h>
#include <VeNo/TypeDefs.h>

namespace VeNo::GUI {
class ActionRegistry {
public:
  ActionRegistry(size_t instanceId);
  ~ActionRegistry();

protected:
  Events::EventCB *addAction(const VString &action);
  void addEditorActions();

protected:
  size_t m_id{0};
  Events::EventHandler *m_handler;
  Vector<VString> m_actions;
  Vector<Ref<Events::EventCB>> m_eventCB;
};
} // namespace VeNo::GUI