#include "WaveForm.h"

#include "VeNo/Core/Config.h"

#include <VeNo/Sound/Tables/WaveLib.h>

namespace VeNo::GUI {

// NOTE: Maybe use OpenGL Directly instead of Path for higher Draw performance
WaveFormComponent::WaveFormComponent(const std::string &name,
                                     const std::string &showName, InstanceID id)
    : BaseComponent(name, showName, id) {}

WaveFormComponent::~WaveFormComponent() noexcept {
  if (!m_HandlerIndex.empty())
    m_handler->removeHandler(m_HandlerIndex);
}

void WaveFormComponent::paint(juce::Graphics &g) {
  auto theme = Core::Config::get().theme();
  g.setColour(theme->getColor(VeNo::Theme::Colors::accent));
  g.strokePath(m_RenderPath, juce::PathStrokeType(0.3));
}

void WaveFormComponent::resized() { createWaveForm(); }

void WaveFormComponent::handle(Events::Event *) {
  m_CurrentTable = m_parameter->getInt() - 1;
  createWaveForm();
}

void WaveFormComponent::createWaveForm() {
  if (getWidth() == 0)
    return;
  m_RenderPath.clear();
  auto &waveLib = Audio::WaveLib::Get();
  auto *table = waveLib.GetWaveTable(m_CurrentTable);
  if (table == nullptr)
    return;
  auto &wave = table->Waves[0];
  float x = 0;
  float xInc = static_cast<float>(getWidth()) / Audio::Wave::TableLength;
  float y = ((wave.Data[0] + 1.0f) / 2.0f) * static_cast<float>(getHeight());
  m_RenderPath.startNewSubPath(x, y);
  for (size_t i = 0; i < Audio::Wave::TableLength; ++i) {
    y = ((wave.Data[i] + 1.0f) / 2.0f) * static_cast<float>(getHeight());
    m_RenderPath.lineTo(x, y);
    x += xInc;
  }
  repaint();
}

void WaveFormComponent::init(int index) {
  m_HandlerIndex = "waveform_" + std::to_string(index);
  m_handler->addHandler(m_HandlerIndex, this);
  m_CurrentTable = m_parameter->getInt() - 1;
  createWaveForm();
}

} // namespace VeNo::GUI