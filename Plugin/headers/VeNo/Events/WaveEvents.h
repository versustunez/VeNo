#pragma once

#include "Event.h"

namespace VeNo::GUI {
class WaveThumbnail;
enum class WaveEditType { RESET = 0, DELETE, DUPLICATE, INTERPOLATE };
class WaveEditEvent : public Events::NoHandleEvent {
public:
  WaveEditEvent(WaveEditType _type, GUI::WaveThumbnail *_thumbnail)
      : type(_type),
        thumbnail(_thumbnail) {}

  WaveEditType type{WaveEditType::RESET};
  GUI::WaveThumbnail *thumbnail{};
};
} // namespace VeNo::GUI