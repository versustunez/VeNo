#pragma once

#include "BaseComponent.h"
#include <JuceHeader.h>

namespace VeNo::GUI {
class Select : public BaseComponent {
public:
  Select(const std::string& name, const std::string& showName, size_t id);
  void resized() override;

protected:
  Ref<juce::ComboBox> m_comboBox;
};
}