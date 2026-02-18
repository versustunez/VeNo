#include "Preset.h"

#include "ScrollComponent.h"
#include "VeNo/Core/Instance.h"
#include "VeNo/Events/GUIEvents.h"
#include "VeNo/GUI/Fonts/Icons.h"

namespace VeNo::GUI {

PresetSaveDialog::PresetSaveDialog(InstanceID id) : BaseComponent("", "", id) {
  m_TextEditor = CreateScope<juce::TextEditor>();
  auto &pm = Core::Instance::get(m_id)->state.PresetManager;
  m_TextEditor->setText(pm->GetCurrentPreset().FilePath);

  m_RandomButton = CreateScope<Button>(m_id);
  m_RandomButton->setIcon(Icons::FontAwesome_Dice);
  m_RandomButton->SetCallback(
      [&] { m_TextEditor->setText(Core::PresetManager::GetRandomFileName()); });

  m_Button = CreateScope<Button>(m_id);
  m_Button->setButtonText("Save");
  m_Button->setFilled(true);
  m_Button->SetCallback([&] {
    pm->GetCurrentPreset().FilePath = m_TextEditor->getText().toStdString();
    pm->Save();
    if (DialogWindow != nullptr)
      DialogWindow->exitModalState(0);
  });

  addAndMakeVisible(*m_Button);
  addAndMakeVisible(*m_TextEditor);
  addAndMakeVisible(*m_RandomButton);
  setSize(400, 75);
}

void PresetSaveDialog::resized() {
  constexpr int randomButtonWidth = 25;
  constexpr int lineHeight = 25;
  constexpr int saveButtonWidth = 100;
  double x = 10;
  double textWidth = getWidth() - randomButtonWidth - saveButtonWidth - (x * 2);

  m_TextEditor->setBounds(x, 40, textWidth, lineHeight);
  m_RandomButton->setBounds(x + textWidth, 40, randomButtonWidth, lineHeight);
  m_Button->setBounds(x + textWidth + randomButtonWidth, 40, saveButtonWidth,
                      lineHeight);
}

void PresetSaveDialog::paint(juce::Graphics &g) {
  g.setColour(Core::Config::get().theme()->getColor(Theme::Colors::font));
  g.drawText("Enter Preset Filename to save", 0,0,getWidth(), 35, juce::Justification::centred);
}

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
    openSaveDialog();
    delete tooltipEvent;
    return;
  } else if (button == m_NextButton.get()) {
    presetManager->LoadNext();
    tooltipEvent->text =
        fmt::format("Loaded Next Preset: {}", presetManager->GetName());
  } else if (button == m_PrevButton.get()) {
    presetManager->LoadPrevious();
    tooltipEvent->text =
        fmt::format("Loaded Previous Preset: {}", presetManager->GetName());
  } else if (button == m_LibraryButton.get()) {
    openLibrary();
    delete tooltipEvent;
    return;
  } else {
    tooltipEvent->text = "Unknown Event Triggered";
  }
  m_handler->triggerEvent("tooltip", tooltipEvent);
}

void PresetComponent::labelTextChanged(juce::Label *) {
  auto &presetManager = Core::Instance::get(m_id)->state.PresetManager;
  presetManager->SetName(m_label->getText().toStdString());
}

void PresetComponent::openLibrary() {
  auto *parent =
      Core::Instance::get(m_id)->mainInterpreter->componentGroup.get();
  auto component = CreateRef<PresetList>(m_name, "", m_id);
  component->setSize(parent->getWidth() - 30, component->getHeight());
  auto viewport = CreateScope<ScrollComponent>("", "", m_id);
  viewport->setFillMode(true);
  viewport->setViewComponent(component);
  viewport->pos.w = component->getWidth();
  viewport->pos.h = 500;
  viewport->setSize(component->getWidth(), 500);
  (void)juce::CallOutBox::launchAsynchronously(std::move(viewport), {0, 0},
                                               parent->getParentComponent());
}

void PresetComponent::openSaveDialog() {
  juce::DialogWindow::LaunchOptions launchOptions{};

  auto *component = new PresetSaveDialog(m_id);
  launchOptions.content =
      juce::OptionalScopedPointer<juce::Component>(component, true);
  launchOptions.resizable = false;
  launchOptions.dialogTitle = "Save Preset";
  launchOptions.escapeKeyTriggersCloseButton = true;
  launchOptions.dialogBackgroundColour =
      Core::Config::get().theme()->getColor(Theme::Colors::bg);
  component->DialogWindow = launchOptions.launchAsync();
}
} // namespace VeNo::GUI
