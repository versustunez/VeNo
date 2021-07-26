#pragma once

#include <JuceHeader.h>
#include <VeNo/Core/Parameter/Handler.h>
#include <VeNo/Core/State/InstanceState.h>
#include <VeNo/GUI/GuiLang/GuiInterpreter.h>

namespace VeNo::Core
{
// Instance has a GetInstance method
struct Instance
{
public:
    size_t id {};
    static Instance* get (size_t m_id);
    static Instance* create();
    static void remove (size_t m_id);
    void init();
    ParameterHandler* handler {};
    juce::AudioProcessorValueTreeState* treeState = nullptr;
    VeNo::State::InstanceState state { id };
    VeNo::GUI::Interpreter mainInterpreter{id};
protected:
    Instance() = default;
    ~Instance();
    static std::vector<Instance*> m_instances;
    juce::AudioProcessorEditor* m_editor {};
};
} // namespace VeNo::Core