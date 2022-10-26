#include "VeNo/Core/Instance.h"

#include <VeNo/GUI/Components/Label.h>
#include <utility>

namespace VeNo::GUI {
Label::Label(std::string name, std::string showName, InstanceID id)
    : BaseComponent(std::move(name), std::move(showName), id) {
  m_LocalLabel = std::make_unique<juce::Label>();
  m_LocalLabel->setJustificationType(juce::Justification::centred);
  addAndMakeVisible(*m_LocalLabel);
}
void Label::setText(std::string text) {
  m_text = std::move(text);
  m_LocalLabel->setText(m_text, juce::NotificationType::dontSendNotification);
}
void Label::setAlign(const std::string &align) {
  if (align == "left") {
    m_LocalLabel->setJustificationType(juce::Justification::left);
  } else if (align == "right") {
    m_LocalLabel->setJustificationType(juce::Justification::right);
  } else if (align == "horizontallyCentred") {
    m_LocalLabel->setJustificationType(juce::Justification::horizontallyCentred);
  } else if (align == "top") {
    m_LocalLabel->setJustificationType(juce::Justification::top);
  } else if (align == "bottom") {
    m_LocalLabel->setJustificationType(juce::Justification::bottom);
  } else if (align == "verticallyCentred") {
    m_LocalLabel->setJustificationType(juce::Justification::verticallyCentred);
  } else if (align == "horizontallyJustified") {
    m_LocalLabel->setJustificationType(juce::Justification::horizontallyJustified);
  } else if (align == "centred") {
    m_LocalLabel->setJustificationType(juce::Justification::centred);
  } else if (align == "centredLeft") {
    m_LocalLabel->setJustificationType(juce::Justification::centredLeft);
  } else if (align == "centredRight") {
    m_LocalLabel->setJustificationType(juce::Justification::centredRight);
  } else if (align == "centredTop") {
    m_LocalLabel->setJustificationType(juce::Justification::centredTop);
  } else if (align == "centredBottom") {
    m_LocalLabel->setJustificationType(juce::Justification::centredBottom);
  } else if (align == "topLeft") {
    m_LocalLabel->setJustificationType(juce::Justification::topLeft);
  } else if (align == "topRight") {
    m_LocalLabel->setJustificationType(juce::Justification::topRight);
  } else if (align == "bottomLeft") {
    m_LocalLabel->setJustificationType(juce::Justification::bottomLeft);
  } else if (align == "bottomRight") {
    m_LocalLabel->setJustificationType(juce::Justification::bottomRight);
  }
}
void Label::resized() { m_LocalLabel->setBounds(0, 0, getWidth(), getHeight()); }

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