#include "../Modulatoren.h"

#include <VeNo/Core/Instance.h>

namespace VeNo::Audio {
void RandomModulator::init(int index) {
  auto *handler = Core::Instance::get(m_id)->handler.get();
  m_name = fmt::format("Random {}", index);
  m_ChangeRate =
      handler->getModulateParameter(fmt::format("rng{}__change_rate", index));
  m_Active = handler->getParameter(fmt::format("rng{}__active", index));
  m_Mode = handler->getParameter(fmt::format("rng{}__mode", index));
  m_Random.setRange(-1, 1);
}

void RandomModulator::update() {
  if (!m_Active->getBool())
    return;
  m_Samples++;
  if (m_Samples > m_ChangeRate->getInt()) {
    int mode = m_Mode->getInt();
    if (mode == 1) {
      m_Value = m_Random.generate();
    } else if (mode == 2) {
      for (int i = 1; i < 5; ++i)
        m_Values[i - 1] = m_Values[i];
      m_Values[4] = m_Random.generate();
      m_Value = 0;
      for (auto &value : m_Values)
        m_Value += value;
      m_Value /= 5;
    } else {
      m_Value =
          std::sin(m_Random.generate() * juce::MathConstants<double>::twoPi);
    }
    m_Samples = 0;
  }
}

double RandomModulator::value(int) { return m_Value; }
} // namespace VeNo::Audio