#include <VeNo/Events/EventHandler.h>

namespace VeNo::Events {
void EventHandler::addEvent(std::string &name, Event *event) {
  m_events[name] = event;
}
void EventHandler::removeEvent(std::string &name) { m_events.erase(name); }
void EventHandler::handle(std::string &name) {
  if (m_events.find(name) != m_events.end())
    m_events[name]->handle();
}
EventHandler::EventHandler(size_t id) : m_id(id) {}
void EventHandler::addHandler(const std::string &name, Handler *handler) {
  DBGN("Registered EventHandler: %s", name.c_str());
  m_handler[name] = handler;
}
void EventHandler::removeHandler(const std::string &name) {
  DBGN("Removed EventHandler: %s", name.c_str());
  m_handler.erase(name);
}
void EventHandler::triggerEvent(const std::string &name, Event &event) {
  if (m_handler.find(name) != m_handler.end())
    m_handler[name]->handle(event);
}
} // namespace VeNo::Events