#pragma once

#include <VeNo/GUI/Components/Knob.h>
#include <VeNo/GUI/Components/PresetList.h>
#include <string>

namespace VeNo::State {
class ComponentStates {
public:
  ~ComponentStates();
  void addKnob(const std::string &name, VeNo::GUI::Knob *knob);
  void removeKnob(const std::string &name);
  std::unordered_map<std::string, VeNo::GUI::Knob *> m_knobs;
  Scope<GUI::PresetList> m_PresetList;
};

} // namespace VeNo::State