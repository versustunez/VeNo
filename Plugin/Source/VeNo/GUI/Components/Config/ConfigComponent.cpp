#include "ConfigComponent.h"

namespace VeNo::GUI {

ConfigComponent::ConfigComponent(const std::string &name,
                                 const std::string &showName, InstanceID id)
    : BaseComponent(name, showName, id) {
  m_config = &Core::Config::get();

  init();
}

Theme::Colors &operator++(Theme::Colors &e) {
  if (e == Theme::Colors::end)
    throw std::out_of_range("for E& operator ++ (E&)");
  e = Theme::Colors(static_cast<std::underlying_type<Theme::Colors>::type>(e) +
                    1);
  return e;
}

void ConfigComponent::init() {
  auto knob = Make<Knob>("gui.scale", "Scale");
  knob->pos.w = getWidth();
  knob->pos.h = 100;
  knob->slider()->addListener(this);
  knob->enableLiveLabel(true);
  knob->setMinMax(0.5, 4.0, 0.1);
  knob->setDefaultValue(1.0);
  knob->setValue(m_config->m_scale);
  m_components.push_back(knob);

  auto author = m_config->properties()->asString(
      "user.author", juce::SystemStats::getFullUserName().toStdString());

  m_AuthorComponent = CreateScope<juce::TextEditor>();
  m_AuthorComponent->setMultiLine(false, false);
  m_AuthorComponent->setText(author);
  m_AuthorComponent->addListener(this);

  addAndMakeVisible(*m_AuthorComponent);

  for (auto i = Theme::Colors::unknown; i < Theme::Colors::end; ++i) {
    if (i == Theme::Colors::unknown)
      continue;
    auto showName = Theme::Theme::colorName(i);
    VUtils::StringUtils::toUpper(showName);
    auto colorItem = Make<ColorChangeComponent>("", showName);
    colorItem->setColorIndex(i);
    colorItem->pos.w = getWidth();
    colorItem->pos.h = 60;
    m_components.push_back(colorItem);
  }
  typedef juce::FlexBox FlexBox;
  auto *flexBox = m_flex.raw();
  flexBox->flexDirection = FlexBox::Direction::column;
  Vector<Ref<ComponentGroup>> groups = {};
  m_flex.setItems(m_components, groups);
}

void ConfigComponent::resized() {
  m_AuthorComponent->setBounds(20, 10, getWidth() - 40, 24);
  m_flex.perform(
      juce::Rectangle<float>(0, 50, (float)getWidth(), (float)getHeight()));
}
void ConfigComponent::calculateSize(int width) {
  Position size{};
  for (auto &component : m_components) {
    size.h += component->pos.h + 2;
  }
  size.h += 30;
  setSize(width, size.h + 30);
}
void ConfigComponent::sliderValueChanged(juce::Slider *) {}
void ConfigComponent::sliderDragEnded(juce::Slider *slider) {
  m_config->setScale(slider->getValue());
}
void ConfigComponent::textEditorTextChanged(juce::TextEditor &editor) {
  m_config->properties()->setValue("user.author", editor.getText().toStdString());
  m_handler->triggerEvent("author.change", new Events::ChangeEvent());
}
} // namespace VeNo::GUI