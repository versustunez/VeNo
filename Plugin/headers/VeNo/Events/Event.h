#pragma once

namespace VeNo::Events {

class Event {
public:
  virtual void handle() = 0;
};

class NoHandleEvent : public Event {
public:
  void handle() override {}
};

class ChangeEvent : public NoHandleEvent {};
class RemoveEvent : public NoHandleEvent {};
class AddEvent : public NoHandleEvent {};

class Handler {
public:
  virtual void handle(Event &event) = 0;
};

} // namespace VeNo::Events