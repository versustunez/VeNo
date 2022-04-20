#include <VUtils/Logging.h>
#include <VeNo/GUI/Components/BaseComponent.h>
#include <VeNo/GUI/Fonts/Fonts.h>
#include <VeNo/Core/Instance.h>

namespace VeNo::GUI {
BaseComponent::BaseComponent(std::string name, std::string showName, InstanceID id)
    : m_name(std::move(name)),
      m_showName(std::move(showName)),
      m_id(id) {

  auto *instance = Core::Instance::get(m_id);
  if (instance) {
    setEventHandler(&instance->eventHandler);
    if (instance->handler->hasParameter(m_name.c_str())) {
      addMouseListener(this, true);
      auto *parameter = instance->handler->getModulateParameter(m_name);
      if (parameter != nullptr) {
        m_parameter = parameter;
        m_mousePicker = CreateScope<GUIEvents::ModulateMouseOpen>(this);
        addMouseListener(m_mousePicker.get(), true);
      } else {
        m_parameter = instance->handler->getParameter(m_name);
      }
    }
  }
}

std::string &BaseComponent::name() { return m_name; }
std::string &BaseComponent::showName() { return m_showName; }

size_t BaseComponent::id() const { return m_id; }
void BaseComponent::createLabel(const std::string &text, bool visible) {
  m_label = std::make_unique<juce::Label>();
  setText(text);
  if (visible)
    addAndMakeVisible(*m_label);
}

void BaseComponent::setText(const std::string &text) {
  if (m_label != nullptr)
    m_label->setText(text, juce::NotificationType::dontSendNotification);
}
Position BaseComponent::resize() {
  const auto &font = m_label->getFont();
  m_label->setFont(font.withHeight(16.0f));
  if (m_label != nullptr) {
    int height = 18;
    int labelHeight = 16;
    int secHeight = getHeight() - height;
    LabelPosition position = m_labelPosition;
    if (position == LabelPosition::TOP) {
      m_label->setBounds(0, 0, getWidth(), labelHeight);
      return {0, height, getWidth(), secHeight};
    } else if (position == LabelPosition::BOTTOM) {
      m_label->setBounds(0, secHeight, getWidth(), labelHeight);
      return {0, 0, getWidth(), secHeight};
    } else if (position == LabelPosition::CENTER) {
      int halfHeight = (getHeight() - height) / 2;
      m_label->setBounds(0, halfHeight, getWidth(), labelHeight);
      return {0, 0, getWidth(), getHeight()};
    }
  }
  return {0, 0, getWidth(), getHeight()};
}
void BaseComponent::setLabelPosition(const std::string &label) {
  if (label == "no")
    m_labelPosition = LabelPosition::NO_LABEL;
  else if (label == "bottom")
    m_labelPosition = LabelPosition::BOTTOM;
  else if (label == "top")
    m_labelPosition = LabelPosition::TOP;
  else if (label == "center")
    m_labelPosition = LabelPosition::CENTER;
  else {
    WARN("Label Position {} is unknown available: no,bottom,top,center",
         label.c_str());
  }
}
std::string &BaseComponent::selectorId() { return m_selectorId; }
void BaseComponent::setSelectorId(std::string selector) {
  m_selectorId = std::move(selector);
}
// Function will be called after the parser is finished
void BaseComponent::afterParsing(Interpreter *) {}
void BaseComponent::triggerAfterParsing(Interpreter *interpreter) {
  if (!m_afterParsingCalled) {
    m_afterParsingCalled = true;
    afterParsing(interpreter);
  }
}
void BaseComponent::setEventHandler(Events::EventHandler *handler) {
  m_handler = handler;
}

int BaseComponent::precision() {
  return m_parameter != nullptr ? m_parameter->precision() : 3;
}
void BaseComponent::mouseEnter(const juce::MouseEvent &) {
  if (m_name.empty())
    return;
  auto tooltipEvent = new Events::TooltipEvent{};
  tooltipEvent->text = Core::Instance::get(id())->handler->getShowName(m_name);
  m_handler->triggerEvent("tooltip", tooltipEvent);
}
void BaseComponent::mouseExit(const juce::MouseEvent &) {
  if (m_name.empty())
    return;
  m_handler->triggerEvent("tooltip", new Events::TooltipEvent{});
}
} // namespace VeNo::GUI
