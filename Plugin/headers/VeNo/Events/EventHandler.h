#pragma once

#include "Event.h"

#include <JuceHeader.h>
#include <VUtils/Logging.h>
#include <VeNo/TypeDefs.h>
#include <string>
#include <unordered_map>

namespace VeNo::Events {
struct QueuedEvent {
  Events::Event* event{};
  std::string name{};
};
class EventHandler : juce::AsyncUpdater {
public:
  explicit EventHandler(size_t id);
  ~EventHandler();
  void addEvent(std::string &name, Event *);
  void removeEvent(std::string &name);
  void handle(std::string &name);

  void addHandler(const std::string &name, Handler *handler);
  void removeHandler(const std::string &name);
  void triggerEvent(const std::string &name, Event* event);

  template <class HandlerClass>
  HandlerClass* create(const std::string& name) {
    auto handler = new HandlerClass();
    m_owningHandlers.push_back(handler);
    addHandler(name, handler);
    return handler;
  }

  template <class HandlerClass>
  HandlerClass* createOrGet(const std::string& name) {
    auto* handler = dynamic_cast<HandlerClass*>(m_handler[name]);
    if (handler == nullptr) {
      handler = new HandlerClass();
      m_owningHandlers.push_back(handler);
      addHandler(name, handler);
    }
    return handler;
  }
public:
  static std::string PARAMETER_CHANGE_KEY;

private:
  void handleAsyncUpdate() override;

protected:
  size_t m_id;
  Map<std::string, Event *> m_events;
  Map<std::string, Handler *> m_handler;
  Vector<Handler*> m_owningHandlers;
  Queue<QueuedEvent> m_queue;
  juce::CriticalSection lock;

  NO_COPY(EventHandler);
};
} // namespace VeNo::Events