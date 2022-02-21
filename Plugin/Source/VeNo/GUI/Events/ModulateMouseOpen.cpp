#include <VeNo/GUI/Components/BaseComponent.h>
#include <VeNo/GUI/Events/ModulateMouseOpen.h>

namespace VeNo::GUIEvents {
ModulateMouseOpen::ModulateMouseOpen(GUI::BaseComponent *baseComponent)
    : m_component(baseComponent) {}

void ModulateMouseOpen::mouseDown(const juce::MouseEvent &event) {
  MouseListener::mouseDown(event);
  if (event.mods.isRightButtonDown())
    openPopupMenu();
}
void ModulateMouseOpen::openPopupMenu() {
  auto component = std::make_unique<juce::TextEditor>();
  component->setMultiLine(true, false);
  component->setReturnKeyStartsNewLine(true);
  component->setSize(300, 600);
  (void)juce::CallOutBox::launchAsynchronously(
      std::move(component), m_component->getScreenBounds(), nullptr);
}
} // namespace VeNo::GUIEvents
