#pragma once

#include <JuceHeader.h>
#include <VeNo/Core/Parameter/Handler.h>
#include <VeNo/Core/State/InstanceState.h>
#include <VeNo/GUI/GuiLang/Interpreter.h>
#include <VeNo/Sound/Synth/Synthesizer.h>

namespace VeNo::Core {
// Instance has a GetInstance method
class Instance {
public:
  size_t id{};
  static Instance *get(size_t m_id);
  static Instance *create();
  static void remove(size_t m_id);
  void init();
  Scope<ParameterHandler> handler{nullptr};
  juce::AudioProcessorValueTreeState *treeState = nullptr;
  State::InstanceState state{id};
  Ref<GUI::Interpreter> mainInterpreter{nullptr};
  Ref<Audio::Synthesizer> synthesizer{nullptr};
  Events::EventHandler eventHandler{id};
  Audio::WaveHolder waveHolder{id};

protected:
  Instance() = default;
  ~Instance();
  static std::vector<Instance *> m_instances;
  juce::AudioProcessorEditor *m_editor{};
};
} // namespace VeNo::Core