#pragma once

#include "BaseComponent.h"
#include "Button.h"

namespace VeNo::GUI {

class PresetListItem : public juce::Component, public Events::Handler {
public:
  PresetListItem(InstanceID id, int presetIndex, bool isHeader);
  ~PresetListItem() override;
  void paint(juce::Graphics &g) override;
  void resized() override;
  void handle(Events::Event *event) override;

protected:
  InstanceID m_ID{};
  int m_PresetIndex{0};
  bool m_IsHeader{false};
  Vector<Scope<Button>> m_Buttons{};
};

class PresetList : public BaseComponent, public Events::Handler {
public:
  PresetList(const std::string &name, const std::string &showName,
             InstanceID id);

  ~PresetList() override;
  void resized() override;

  void handle(Events::Event *event) override;

protected:
  Vector<Scope<PresetListItem>> m_Items;
};

} // namespace VeNo::GUI
