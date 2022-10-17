#pragma once

#include <JuceHeader.h>
#include <VeNo/Core/Parameter/Handler.h>
#include <VeNo/Core/State/InstanceState.h>
#include <VeNo/GUI/GuiLang/Interpreter.h>
#include <VeNo/Sound/Buffer.h>
#include <VeNo/Sound/Synth/Synthesizer.h>

namespace VeNo::Core {
// Instance has a GetInstance method
class Instance {
public:
  InstanceID id;
  double bpm{120};

public:
  static Instance *get(InstanceID m_id);
  static Instance *create();
  static void remove(InstanceID m_id);
  static State::InstanceState *getState(InstanceID id);
  static void TriggerEventOnAllInstances(const char *name);
  void init();
  Scope<ParameterHandler> handler{nullptr};
  juce::AudioProcessorValueTreeState *treeState = nullptr;
  State::InstanceState state{id};
  Ref<GUI::Interpreter> mainInterpreter{nullptr};
  Ref<Audio::Synthesizer> synthesizer{nullptr};
  Ref<Audio::Buffer> buffer{nullptr};
  Events::EventHandler eventHandler{id};

protected:
  Instance(InstanceID _id) : id(_id) {}
  ~Instance();
  static std::vector<Instance *> m_instances;
  juce::AudioProcessorEditor *m_editor{};

protected:
  static Mutex s_CreateInstanceGuard;
};
} // namespace VeNo::Core