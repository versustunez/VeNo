#include "VeNo/GUI/Components/Matrix.h"
#include "VeNo/GUI/Components/ScrollComponent.h"

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
  auto component = CreateRef<GUI::MatrixComponent>(m_component->name(), m_component->id());
  auto viewport = CreateScope<GUI::ScrollComponent>("", "", m_component->id());
  viewport->setViewComponent(component);
  viewport->pos.w = component->getWidth();
  viewport->pos.h = 400;
  viewport->setSize(component->getWidth(), 400);
  (void)juce::CallOutBox::launchAsynchronously(
      std::move(viewport), m_component->getScreenBounds(), nullptr);
}
} // namespace VeNo::GUIEvents
