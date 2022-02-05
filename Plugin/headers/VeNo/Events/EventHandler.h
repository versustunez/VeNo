#pragma once

#include "Event.h"
#include "VUtils/Logging.h"
#include <VeNo/TypeDefs.h>
#include <string>
#include <unordered_map>

namespace VeNo::Events {
class EventHandler {
public:
  explicit EventHandler(size_t id);
  void addEvent(std::string &name, Event *);
  void removeEvent(std::string &name);
  void handle(std::string &name);

  void addHandler(const std::string &name, Handler *handler);
  void removeHandler(const std::string &name);
  void triggerEvent(const std::string &name, Event &event);

protected:
  size_t m_id;
  Map<std::string, Event *> m_events;
  Map<std::string, Handler *> m_handler;
};
} // namespace VeNo::Events