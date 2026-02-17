#include "PresetList.h"

#include "VeNo/Core/Config.h"
#include "VeNo/Core/Instance.h"
#include "VeNo/Events/GUIEvents.h"
#include "VeNo/GUI/Fonts/Icons.h"

#include <VeNo/Core/PresetManager.h>

namespace VeNo::GUI {

PresetListItem::PresetListItem(InstanceID id, int presetIndex, bool isHeader)
    : m_ID(id),
      m_PresetIndex(presetIndex),
      m_IsHeader(isHeader) {

  auto key = std::format("preset-item-{}", presetIndex);
  Core::Instance::get(m_ID)->eventHandler.addHandler(key, this);
  if (m_PresetIndex == -1) {
    {
      auto button = CreateScope<Button>("init_button", "", m_ID);
      button->setButtonText("Init");
      button->setAction(key);
      m_Buttons.push_back(std::move(button));
    }
    {
      auto button = CreateScope<Button>("", "", m_ID);
      button->setButtonText("Reload");
      button->setAction("reload-preset-db");
      m_Buttons.push_back(std::move(button));
    }
  } else if (presetIndex >= 0) {
    {
      auto button = CreateScope<Button>("load_preset", "", m_ID);
      button->setIcon(Icons::FontAwesome_Check);
      button->setAction(key);
      m_Buttons.push_back(std::move(button));
    }
  }

  for (const auto &m_Button : m_Buttons)
    addAndMakeVisible(*m_Button);
}

void PresetListItem::paint(juce::Graphics &g) {
  double onePercent = getWidth() / 100.0;
  int indexContainerWidth = onePercent * 10.0;
  int nameContainerWidth = onePercent * 50.0;
  int authorButtonContainerWidth = onePercent * 20.0;
  auto *theme = Core::Config::get().theme().get();
  g.setColour(theme->getColor(Theme::Colors::font));
  if (m_PresetIndex == -1) {
    g.fillAll(theme->getColor(Theme::Colors::root));
    g.setFont(9.0f);
    g.drawText(std::format("Preset Path: {}", Core::Config::get().presetPath),
               10, 0, getWidth(), 11, juce::Justification::verticallyCentred);
    g.setFont(16.0f);
    g.drawText("Nr.", 0, 0, indexContainerWidth, getHeight(),
               juce::Justification::centred);
    g.drawText("Name", indexContainerWidth, 0, nameContainerWidth, getHeight(),
               juce::Justification::verticallyCentred);
    g.drawText("Author", indexContainerWidth + nameContainerWidth, 0,
               authorButtonContainerWidth, getHeight(),
               juce::Justification::verticallyCentred);
  } else {
    auto index =
        (m_PresetIndex & 1) == 0 ? Theme::Colors::bg : Theme::Colors::bgTwo;
    g.fillAll(theme->getColor(index));
    Core::Preset &preset =
        Core::PresetManager::GetPresetDatabase().Presets[(size_t)m_PresetIndex];
    g.drawText(std::to_string(m_PresetIndex+1), 0, 0, indexContainerWidth,
               getHeight(), juce::Justification::centred);
    g.drawText(preset.Name, indexContainerWidth, 0, nameContainerWidth,
               getHeight(), juce::Justification::verticallyCentred);
    g.drawText(preset.Author, indexContainerWidth + nameContainerWidth, 0,
               authorButtonContainerWidth, getHeight(),
               juce::Justification::verticallyCentred);
  }
  g.setColour(
      theme->getColor(m_IsHeader ? Theme::Colors::accent : Theme::Colors::font)
          .withAlpha(0.4f));
  g.drawLine(0, getHeight() - 1, getWidth(), getHeight() - 1);
}

void PresetListItem::resized() {
  float onePercent = (float)getWidth() / 100.0f;
  float authorButtonContainerWidth = onePercent * 15.0f;
  float x = onePercent * 85.0f;
  int size = m_Buttons.size() > 1 ? m_Buttons.size() : 2;
  float xOff = (authorButtonContainerWidth - (size * 10))  / size;
  double y = getHeight() / 2.0;
  for (size_t i = 0; i < m_Buttons.size(); ++i)
    m_Buttons[i]->setBounds(x + (xOff * i + 10), y / 2.0, xOff, y);
}

void PresetListItem::handle(Events::Event *event) {
  auto *clickEvent = event->as<Events::ButtonClickedEvent>();
  if (!clickEvent)
    return;
  auto *instance = Core::Instance::get(m_ID);
  auto &presetManager = instance->state.PresetManager;
  auto &name = clickEvent->button->name();
  if (name == "init_button") {
    presetManager->Reset();
    instance->eventHandler.triggerEvent(
        "tooltip", new Events::TooltipEvent("Loaded Init Preset"));
  } else if (name == "load_preset") {
    presetManager->LoadIndexPreset((size_t)m_PresetIndex);
    auto tooltipText = std::format("Loaded Preset: {}",
                                   presetManager->GetCurrentPreset().Name);
    instance->eventHandler.triggerEvent(
        "tooltip", new Events::TooltipEvent(tooltipText.c_str()));
  }
}

PresetListItem::~PresetListItem() {
  Core::Instance::get(m_ID)->eventHandler.removeHandler(
      std::format("preset-item-{}", m_PresetIndex));
}

PresetList::PresetList(const std::string &name, const std::string &showName,
                       InstanceID id)
    : BaseComponent(name, showName, id) {
  auto &presets = Core::PresetManager::GetPresetDatabase().Presets;
  m_Items.emplace_back(CreateScope<PresetListItem>(id, -1, true));
  addAndMakeVisible(*m_Items[0]);
  for (size_t i = 0; i < presets.size(); i++) {
    m_Items.push_back(CreateScope<PresetListItem>(id, i, false));
    addAndMakeVisible(*m_Items[i + 1]);
  }
  setSize(WINDOW_WIDTH, int(m_Items.size()) * 50 + 20);
  m_handler->addHandler("reload-preset-db", this);
}

PresetList::~PresetList() { m_handler->removeHandler("reload-preset-db"); }

void PresetList::resized() {
  for (size_t i = 0; i < m_Items.size(); ++i) {
    m_Items[i]->setBounds(10, (int)i * 50 + 10, getWidth() - 20, 50);
  }
}

void PresetList::handle(Events::Event *) {
  removeAllChildren();
  Core::PresetManager::InitDatabaseFromDisk();
  m_Items.clear();
  auto &presets = Core::PresetManager::GetPresetDatabase().Presets;
  m_Items.emplace_back(CreateScope<PresetListItem>(m_id, -1, true));
  addAndMakeVisible(*m_Items[0]);
  for (size_t i = 0; i < presets.size(); i++) {
    m_Items.push_back(CreateScope<PresetListItem>(m_id, i, false));
    addAndMakeVisible(*m_Items[i + 1]);
  }
  setSize(getWidth(), int(m_Items.size()) * 50 + 20);
  resized();
}
} // namespace VeNo::GUI