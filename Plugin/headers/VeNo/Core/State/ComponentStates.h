#pragma once

#include <VeNo/GUI/Components/Knob.h>
#include <string>
#include <vendor/tsl/robin_map.h>
namespace VeNo::State
{
class ComponentStates
{
public:
    ~ComponentStates();
    void addKnob (const std::string& name, VeNo::GUI::Knob* knob);
    void removeKnob (const std::string& name);
    tsl::robin_map<std::string, VeNo::GUI::Knob*> m_knobs;
};

} // namespace VeNo::State