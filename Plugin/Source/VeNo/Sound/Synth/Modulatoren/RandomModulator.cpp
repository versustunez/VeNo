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
      for (auto &value : m_Value) {
        value = m_Random.generate();
      }
    } else if (mode == 2) {
      for (size_t i = 0; i < VOICES; i++) {
        for (int j = 1; j < HISTORY_SIZE; ++j)
          m_Values[i][j - 1] = m_Values[i][j];
        m_Values[i][4] = m_Random.generate();
        float result = 0;
        for (auto &value : m_Values[i])
          result += value;
        m_Value[i] = result / m_Values[i].size();
      }

    } else {
      for (auto &value : m_Value) {
        value = std::sin(m_Random.generate() * juce::MathConstants<double>::twoPi);
      }
    }
    m_Samples = 0;
  }
}

double RandomModulator::value(int index) {
  return m_Value[index];
}
} // namespace VeNo::Audio
