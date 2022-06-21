#pragma once

#include "BaseComponent.h"

namespace VeNo::GUI {

class WaveFormComponent : public BaseComponent, public Events::Handler {
public:
  WaveFormComponent(const std::string &name, const std::string &showName,
                    InstanceID id);

  ~WaveFormComponent() override;

  void paint(juce::Graphics &g) override;
  void resized() override;

  void handle(Events::Event *event) override;
  void createWaveForm();
  void init(int index);

protected:
  juce::Path m_RenderPath{};
  int m_CurrentTable{0};
  std::string m_HandlerIndex{};
};
}