#pragma once
#include <string>

namespace VeNo::Events {

class Event {
public:
  virtual ~Event() = default;
  virtual void handle() {}
  template <class AsClass> AsClass *as() {
    return dynamic_cast<AsClass *>(this);
  }
  template <class AsClass> bool is() {
    return dynamic_cast<AsClass *>(this) != nullptr;
  }
};

class NoHandleEvent : public Event {
public:
  void handle() override {}
};

class ChangeEvent : public NoHandleEvent {};
class TriggerEvent : public NoHandleEvent {};
class ParameterChangeEnd : public NoHandleEvent {};
class TooltipEvent : public NoHandleEvent {
public:
  TooltipEvent() = default;
  TooltipEvent(const char *_text) : text(_text) {}
  std::string text;
};
class ParameterChange : public NoHandleEvent {
public:
  std::string name{};
  double value{0.0};
};
class ValueChange : public NoHandleEvent {
public:
  explicit ValueChange(double val) : value(val) {}
  double value{0.0};
};
class RemoveEvent : public NoHandleEvent {};
class AddEvent : public NoHandleEvent {};
class MatrixChangeEvent : public NoHandleEvent {
public:
  explicit MatrixChangeEvent(std::string key, bool isDeleted = false)
      : Key(std::move(key)),
        IsDeleted(isDeleted) {}
  std::string Key{};
  bool IsDeleted{false};
};

class Handler {
public:
  virtual ~Handler() = default;
  virtual void handle(Event *event) = 0;
};

} // namespace VeNo::Events