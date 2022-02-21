#pragma once
#include <string>

namespace VeNo::Events {

class Event {
public:
  virtual ~Event() = default;
  virtual void handle(){};
};

class NoHandleEvent : public Event {
public:
  void handle() override{};
};

class ChangeEvent : public NoHandleEvent {};
class TooltipEvent : public NoHandleEvent {
public:
  std::string text;
};
class RemoveEvent : public NoHandleEvent {};
class AddEvent : public NoHandleEvent {};

class Handler {
public:
  virtual ~Handler() = default;
  virtual void handle(Event *event) = 0;
};

} // namespace VeNo::Events