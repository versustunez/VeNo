#include "LCDListener.h"

#include "VeNo/Core/Instance.h"
#include "VeNo/GUI/Components/Knob.h"

namespace VeNo::GUI {
LCDListener::LCDListener(InstanceID id) : m_id(id){};
void LCDListener::sliderValueChanged(juce::Slider *slider) {
  if (!m_eventHandler)
    init();
  m_eventHandler->triggerEvent("lcd-change",
                               new Events::ValueChange(slider->getValue()));
}

void LCDListener::sliderDragStarted(juce::Slider *slider) {
  if (!m_eventHandler)
    init();
  auto *event = new Events::ParameterChange();
  auto *base = (BaseComponent *)slider->getParentComponent();
  event->name = base->showName();
  event->value = slider->getValue();
  m_eventHandler->triggerEvent("lcd-change", event);
}

void LCDListener::sliderDragEnded(juce::Slider *slider) {
  if (!m_eventHandler)
    init();
  m_eventHandler->triggerEvent("lcd-change", new Events::ParameterChangeEnd);
}
void LCDListener::init() {
  m_eventHandler = &Core::Instance::get(m_id)->eventHandler;
}
} // namespace VeNo::GUI
