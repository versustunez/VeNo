#pragma once

#include "ComponentStates.h"

#include <VeNo/GUI/ActionRegistry.h>
#include <VeNo/Window/WaveEditor.h>
#include <cstdlib>
namespace VeNo::State {
class InstanceState {
public:
  explicit InstanceState(size_t id);
  ~InstanceState();
  ComponentStates components;
  size_t m_id;
  Scope<juce::Drawable> logo;
  Scope<Windows::WaveEditorWindow> waveEditorWindow{};
  Scope<GUI::ActionRegistry> actionRegistry;
};
} // namespace VeNo::State