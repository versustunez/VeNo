#pragma once

#include "../PresetManager.h"
#include "ComponentStates.h"
#include "VeNo/Sound/DSP/FXChain.h"

#include <VeNo/GUI/ActionRegistry.h>
#include <VeNo/GUI/Events/LCDListener.h>
#include <cstdlib>

namespace VeNo::State {
class InstanceState {
public:
  explicit InstanceState(InstanceID id);
  ~InstanceState();
  ComponentStates components;
  InstanceID m_id;
  Scope<juce::Drawable> logo;
  Scope<GUI::ActionRegistry> actionRegistry;
  Scope<GUI::LCDListener> lcdListener;
  Scope<Core::PresetManager> PresetManager;
  Scope<Audio::FXChain> FXChain;
  void RegisterSynth();
};
} // namespace VeNo::State