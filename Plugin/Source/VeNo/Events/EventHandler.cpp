#include <VeNo/Events/EventHandler.h>

namespace VeNo::Events
{
void EventHandler::addEvent (std::string& name, Event* event)
{
    m_events[name] = event;
}
void EventHandler::removeEvent (std::string& name)
{
    m_events.erase (name);
}
void EventHandler::handle (std::string& name)
{
    if (m_events.find(name) != m_events.end())
        m_events[name]->handle();
}
EventHandler::EventHandler (size_t id) : m_id(id)
{
}
} // namespace VeNo::Events