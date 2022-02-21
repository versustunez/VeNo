#pragma once

#include <JuceHeader.h>

namespace VeNo::GUI {
class BaseComponent;
}

namespace VeNo::GUIEvents {
class ModulateMouseOpen : public juce::MouseListener {
public:
  explicit ModulateMouseOpen(GUI::BaseComponent *mComponent);
  void mouseDown(const juce::MouseEvent &event) override;
  void openPopupMenu();

private:
  GUI::BaseComponent *m_component;
};
} // namespace VeNo::GUIEvents