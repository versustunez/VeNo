#include <VeNo/Events/EventHandler.h>

namespace VeNo::Events {
std::string EventHandler::PARAMETER_CHANGE_KEY = "parameter-changed";
void EventHandler::addEvent(std::string &name, Event *event) {
  m_events[name] = event;
}
void EventHandler::removeEvent(std::string &name) { m_events.erase(name); }
void EventHandler::handle(std::string &name) {
  if (m_events.find(name) != m_events.end())
    m_events[name]->handle();
}
EventHandler::EventHandler(InstanceID id) : m_id(id) {
  DBG("Created new EventHandler for id: " << id << "\n");
}
EventHandler::~EventHandler() {
  cancelPendingUpdate();
  for (auto *owningHandler : m_owningHandlers) {
    delete owningHandler;
  }
  m_owningHandlers.clear();
}
// @TODO: Allow Multi Handle for Same Parameter
void EventHandler::addHandler(const std::string &name, Handler *handler) {
  juce::ScopedLock local_lock(m_handlerLock);
  m_handler[name] = handler;
}

void EventHandler::removeHandler(const std::string &name) {
  juce::ScopedLock local_lock(m_handlerLock);
  DBGN("Removed EventHandler: {}", name.c_str());
  m_handler.erase(name);
}

void EventHandler::triggerEvent(const std::string &name, Event *event) {
  juce::ScopedLock local_lock(m_queueLock);
  m_queue.push(QueuedEvent{event, name});
  if (!isUpdatePending()) {
    triggerAsyncUpdate();
  }
}
void EventHandler::handleAsyncUpdate() {
  Queue<QueuedEvent> queue;
  {
    juce::ScopedLock local_lock(m_queueLock);
    queue = m_queue;
    m_queue = {};
  }
  while (!queue.empty()) {
    juce::ScopedLock local_lock(m_handlerLock);
    auto &front = queue.front();
    if (m_handler.find(front.name) != m_handler.end())
      m_handler[front.name]->handle(front.event);
    delete front.event;
    queue.pop();
  }
}
} // namespace VeNo::Events