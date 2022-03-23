#include "VeNo/Core/Instance.h"

#include <VeNo/GUI/Components/Label.h>
#include <VeNo/GUI/GUIUtils.h>
#include <utility>

namespace VeNo::GUI {
Label::Label(std::string name, std::string showName, size_t id)
    : BaseComponent(std::move(name), std::move(showName), id) {
  m_label = std::make_unique<juce::Label>();
  m_label->setJustificationType(juce::Justification::centred);
  addAndMakeVisible(*m_label);
}
void Label::setText(std::string text) {
  m_text = std::move(text);
  m_label->setText(m_text, juce::NotificationType::dontSendNotification);
}
void Label::setAlign(const std::string &align) {
  if (align == "left") {
    m_label->setJustificationType(juce::Justification::left);
  } else if (align == "right") {
    m_label->setJustificationType(juce::Justification::right);
  } else if (align == "horizontallyCentred") {
    m_label->setJustificationType(juce::Justification::horizontallyCentred);
  } else if (align == "top") {
    m_label->setJustificationType(juce::Justification::top);
  } else if (align == "bottom") {
    m_label->setJustificationType(juce::Justification::bottom);
  } else if (align == "verticallyCentred") {
    m_label->setJustificationType(juce::Justification::verticallyCentred);
  } else if (align == "horizontallyJustified") {
    m_label->setJustificationType(juce::Justification::horizontallyJustified);
  } else if (align == "centred") {
    m_label->setJustificationType(juce::Justification::centred);
  } else if (align == "centredLeft") {
    m_label->setJustificationType(juce::Justification::centredLeft);
  } else if (align == "centredRight") {
    m_label->setJustificationType(juce::Justification::centredRight);
  } else if (align == "centredTop") {
    m_label->setJustificationType(juce::Justification::centredTop);
  } else if (align == "centredBottom") {
    m_label->setJustificationType(juce::Justification::centredBottom);
  } else if (align == "topLeft") {
    m_label->setJustificationType(juce::Justification::topLeft);
  } else if (align == "topRight") {
    m_label->setJustificationType(juce::Justification::topRight);
  } else if (align == "bottomLeft") {
    m_label->setJustificationType(juce::Justification::bottomLeft);
  } else if (align == "bottomRight") {
    m_label->setJustificationType(juce::Justification::bottomRight);
  }
}
void Label::resized() { m_label->setBounds(0, 0, getWidth(), getHeight()); }

void Label::addListenEvent(const std::string &event) {
  m_eventListener = CreateScope<LabelEventListener>(event, this);
  auto &eventHandler = Core::Instance::get(id())->eventHandler;
  eventHandler.addHandler(event, m_eventListener.get());
}

LabelEventListener::LabelEventListener(std::string name, Label *label)
    : m_label(label),
      m_name(std::move(name)) {}
LabelEventListener::~LabelEventListener() {
  auto &eventHandler = Core::Instance::get(m_label->id())->eventHandler;
  eventHandler.removeHandler(m_name);
}

void LabelEventListener::handle(Events::Event *event) {
  auto *tooltipEventOrNull = event->as<Events::TooltipEvent>();
  if (tooltipEventOrNull != nullptr) {
    m_label->setText(tooltipEventOrNull->text);
  }
}

} // namespace VeNo::GUI