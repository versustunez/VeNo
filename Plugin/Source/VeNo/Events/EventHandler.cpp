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
EventHandler::EventHandler(size_t id) : m_id(id) {}
EventHandler::~EventHandler() noexcept {
  for (auto *owningHandler : m_owningHandlers) {
    delete owningHandler;
  }
  m_owningHandlers.clear();
}
void EventHandler::addHandler(const std::string &name, Handler *handler) {
  DBGN("[%p], Registered EventHandler: %s", this, name.c_str());
  m_handler[name] = handler;
}
void EventHandler::removeHandler(const std::string &name) {
  DBGN("Removed EventHandler: %s", name.c_str());
  m_handler.erase(name);
}
void EventHandler::triggerEvent(const std::string &name, Event *event) {
  m_queue.push(QueuedEvent{event, name});
  cancelPendingUpdate();
  triggerAsyncUpdate();
}
void EventHandler::handleAsyncUpdate() {
  while (!m_queue.empty()) {
    auto &front = m_queue.front();
    if (m_handler.find(front.name) != m_handler.end())
      m_handler[front.name]->handle(front.event);
    delete front.event;
    m_queue.pop();
  }
}
} // namespace VeNo::Events