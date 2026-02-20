#include "../Modulatoren.h"
#include "VUtils/Math.h"

#include <VeNo/Core/Instance.h>

namespace VeNo::Audio {
void LFOModulator::init(int number) {
  m_name = fmt::format("LFO {}", number);
  m_WaveLib = &WaveLib::Get();
  auto *instance = Core::Instance::get(m_id);
  if (instance == nullptr)
    return;

  // Setup LFO-Data ;)
  auto *handler = instance->handler.get();
  m_Data.Frequency =
      handler->getModulateParameter(fmt::format("lfo{}__rate", number));
  m_Data.Active = handler->getParameter(fmt::format("lfo{}__active", number));
  m_Data.WaveForm = handler->getParameter(fmt::format("lfo{}__wave", number));
  m_Data.Table = m_WaveLib->GetWaveTable(0);
}

static void renderLFOVoice(LFOData &data) {
  auto &table = data.Table->Waves[0];
  data.Phase += data.FreqInc;
  data.Phase -= float(data.Phase >= 1.0);
  double val = data.Phase * static_cast<double>(Wave::TableLength);
  int value = (int)val;
  int temp = (int)val + 1;
  double sum = table.Data[value];
  double sum2 = table.Data[temp];
  double fraction = val - value;
  data.Output = VUtils::Math::lerp(sum, sum2, fraction);
}

void LFOModulator::update() {
  if (!m_Data.Active->getBool()) {
    m_Data.Output = 0.0;
    return;
  }
  m_Data.FreqInc = m_Data.Frequency->getValue() / m_SampleRate;
  int wave = m_Data.WaveForm->getInt();
  if (wave != m_Data.LastWaveForm) {
    m_Data.LastWaveForm = wave;
    m_Data.Table = m_WaveLib->GetWaveTable(wave - 1);
  }
  renderLFOVoice(m_Data);
}
double LFOModulator::value(int) { return m_Data.Output; }

} // namespace VeNo::Audio
