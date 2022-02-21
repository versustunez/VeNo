#pragma once

#include "BaseComponent.h"

#include <JuceHeader.h>

namespace VeNo::GUI {
class TabbedComponent : public BaseComponent {
public:
  TabbedComponent(const std::string &name, const std::string &showName,
                  size_t id);
};
} // namespace VeNo::GUI