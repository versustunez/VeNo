#pragma once

#include <JuceHeader.h>
#include <VeNo/Core/Parameter/Handler.h>
#include <VeNo/Core/State/InstanceState.h>
#include <VeNo/GUI/GuiLang/Interpreter.h>
#include <VeNo/Sound/Synth/Synthesizer.h>
#include <VeNo/Sound/Buffer.h>

namespace VeNo::Core {
// Instance has a GetInstance method
class Instance {
public:
  static Instance *get(InstanceID m_id);
  static Instance *create();
  static void remove(InstanceID m_id);
  static State::InstanceState* getState(InstanceID id);
  void init();
  Scope<ParameterHandler> handler{nullptr};
  juce::AudioProcessorValueTreeState *treeState = nullptr;
  State::InstanceState state{id};
  Ref<GUI::Interpreter> mainInterpreter{nullptr};
  Ref<Audio::Synthesizer> synthesizer{nullptr};
  Ref<Audio::Buffer> buffer{nullptr};
  Events::EventHandler eventHandler{id};

public:
  InstanceID id{};
  double bpm{120};
protected:
  Instance() = default;
  ~Instance();
  static std::vector<Instance *> m_instances;
  juce::AudioProcessorEditor *m_editor{};
};
} // namespace VeNo::Core