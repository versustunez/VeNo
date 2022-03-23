#include <VeNo/Events/EventCB.h>

namespace VeNo::Events {
void EventCB::handle(Event *event) {
  if (m_cb) {
    m_cb(event);
  }
}
void EventCB::setCB(std::function<void(Event *)> lambda) { m_cb = std::move(lambda); }
} // namespace VeNo::Events