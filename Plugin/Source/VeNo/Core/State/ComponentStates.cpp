#include <VeNo/Core/State/ComponentStates.h>
namespace VeNo::State
{
ComponentStates::~ComponentStates()
{
    m_knobs.clear();
}
void ComponentStates::addKnob (const std::string& name, VeNo::GUI::Knob* knob)
{
    m_knobs[name] = knob;
}
void ComponentStates::removeKnob (const std::string& name)
{
    if (m_knobs.find (name) != m_knobs.end())
        m_knobs.erase (name);
}
} // namespace VeNo::State
