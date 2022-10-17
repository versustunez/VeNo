#include "Preset.h"

#include "VeNo/Core/Instance.h"
#include "VeNo/Events/GUIEvents.h"
#include "VeNo/GUI/Fonts/Icons.h"

namespace VeNo::GUI {

PresetComponent::PresetComponent(const std::string &name,
                                 const std::string &showName, InstanceID id)
    : BaseComponent(name, showName, id) {

  auto &presetManager = Core::Instance::get(m_id)->state.PresetManager;

  m_SaveButton = VComponent::create<Button>(id);
  m_NextButton = VComponent::create<Button>(id);
  m_PrevButton = VComponent::create<Button>(id);
  m_LibraryButton = VComponent::create<Button>(id);
  createLabel(presetManager->GetName(), true);
  m_label->addListener(this);
  m_label->setEditable(true);
  m_SaveButton->setIcon(VeNo::GUI::Icons::FontAwesome_Save);
  m_NextButton->setIcon(VeNo::GUI::Icons::FontAwesome_AngleRight);
  m_PrevButton->setIcon(VeNo::GUI::Icons::FontAwesome_AngleLeft);
  m_LibraryButton->setIcon(VeNo::GUI::Icons::FontAwesome_List);
  m_SaveButton->setAction("preset_button_clicked");
  m_NextButton->setAction("preset_button_clicked");
  m_PrevButton->setAction("preset_button_clicked");
  m_LibraryButton->setAction("preset_button_clicked");
  addAndMakeVisible(*m_SaveButton);
  addAndMakeVisible(*m_NextButton);
  addAndMakeVisible(*m_PrevButton);
  addAndMakeVisible(*m_LibraryButton);
}

PresetComponent::~PresetComponent() {
  m_handler->removeHandler("preset_button_clicked");
  m_handler->removeHandler("preset_changed");
}

void PresetComponent::resized() {
  int iconWidth = 24;
  auto editorWidth = getWidth() - (iconWidth * 4);
  m_label->setBounds(0, 0, editorWidth, getHeight());
  m_LibraryButton->setBounds(editorWidth, 0, iconWidth, getHeight());
  m_SaveButton->setBounds(editorWidth + iconWidth, 0, iconWidth, getHeight());
  m_PrevButton->setBounds(editorWidth + (iconWidth * 2), 0, iconWidth,
                          getHeight());
  m_NextButton->setBounds(editorWidth + (iconWidth * 3), 0, iconWidth,
                          getHeight());
}

void PresetComponent::init() {
  m_handler->addHandler("preset_button_clicked", this);
  m_handler->addHandler("preset_changed", this);
}

void PresetComponent::handle(Events::Event *event) {
  auto &presetManager = Core::Instance::get(m_id)->state.PresetManager;
  if (event->is<Events::ChangeEvent>()) {
    // Preset has changed
    setText(presetManager->GetName());
    return;
  }
  auto *realEvent = event->as<Events::ButtonClickedEvent>();
  if (!realEvent)
    return;
  auto tooltipEvent = new Events::TooltipEvent{};
  auto *button = realEvent->button;
  if (button == m_SaveButton.get()) {
    presetManager->Save();
    tooltipEvent->text =
        fmt::format("Saved Preset: {}", presetManager->GetName());
  } else if (button == m_NextButton.get()) {
    presetManager->LoadNext();
    tooltipEvent->text =
        fmt::format("Loaded Next Preset: {}", presetManager->GetName());
  } else if (button == m_PrevButton.get()) {
    presetManager->LoadPrevious();
    tooltipEvent->text =
        fmt::format("Loaded Previous Preset: {}", presetManager->GetName());
  } else {
    tooltipEvent->text = "Unknown Event Triggered";
  }
  m_handler->triggerEvent("tooltip", tooltipEvent);
}
void PresetComponent::labelTextChanged(juce::Label *) {
  auto &presetManager = Core::Instance::get(m_id)->state.PresetManager;
  presetManager->SetName(m_label->getText().toStdString());
}
} // namespace VeNo::GUI