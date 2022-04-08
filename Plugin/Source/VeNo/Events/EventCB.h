#pragma once
#include "Event.h"

#include <functional>

namespace VeNo::Events {
class EventCB : public Handler {
public:
  // set Callback function ;)
  void setCB(std::function<void(Event *)> lambda);
  void handle(Event *event) override;

protected:
  std::function<void(Event *)> m_cb;
};
} // namespace VeNo::Events