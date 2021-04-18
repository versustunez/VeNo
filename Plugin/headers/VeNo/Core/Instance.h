#pragma once

#include <JuceHeader.h>
#include <VeNo/Core/Parameter/Handler.h>
#include <VeNo/Core/State/InstanceState.h>

namespace VeNo::Core
{
// Instance has a GetInstance method
struct Instance
{
    size_t id {};

public:
    static Instance* get (size_t m_id);
    static Instance* create();
    static void remove (size_t m_id);
    void init();
    ParameterHandler* handler {};
    juce::AudioProcessorValueTreeState* treeState = nullptr;
    VeNo::State::InstanceState state { id };

protected:
    Instance() = default;
    ~Instance();
    static std::vector<Instance*> m_instances;
    juce::AudioProcessorEditor* m_editor {};
};
} // namespace VeNo::Core