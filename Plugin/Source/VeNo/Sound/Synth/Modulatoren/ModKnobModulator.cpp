#include "../Modulatoren.h"

#include <VeNo/Core/Instance.h>
namespace VeNo::Audio {

void ModKnobsModulator::init(int index) {
  auto *handler = Core::Instance::get(m_id)->handler.get();
  m_Parameter = handler->getParameter(fmt::format("mod{}", index));
}

void ModKnobsModulator::update() { m_Value = m_Parameter->getValue(); }

double ModKnobsModulator::value(int) { return m_Value; }
void ModKnobsModulator::init(const VString &name, int) {
  auto *handler = Core::Instance::get(m_id)->handler.get();
  m_Parameter = handler->getParameter(name);
}

} // namespace VeNo::Audio
