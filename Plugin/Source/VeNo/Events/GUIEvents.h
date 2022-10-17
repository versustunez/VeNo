#pragma once

#include "../GUI/Components/Button.h"
#include "Event.h"

namespace VeNo::Events {
class ButtonClickedEvent : public NoHandleEvent {
public:
  explicit ButtonClickedEvent(GUI::Button *_button) : button(_button) {}
  GUI::Button *button;
};

} // namespace VeNo::Events