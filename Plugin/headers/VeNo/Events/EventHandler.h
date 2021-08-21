#pragma once

#include "Event.h"
#include <string>
#include <unordered_map>
namespace VeNo::Events
{
class EventHandler
{
public:
    explicit EventHandler(size_t id);
    void addEvent(std::string& name, Event*);
    void removeEvent(std::string& name);
    void handle(std::string& name);
protected:
    size_t m_id;
    std::unordered_map<std::string, Event*> m_events;
};
} // namespace VeNo::Events