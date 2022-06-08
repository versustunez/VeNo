#pragma once

#include "../BaseComponent.h"

#include <VeNo/Events/Event.h>
#include <VeNo/GUI/Theme/Theme.h>

namespace VeNo::GUI {

enum class LCDInfoState { VU = 0, WAVE, STEREO, ONCHANGE, NOTHING };

class LCDHeader : public BaseComponent {
public:
  LCDHeader(const std::string &name, const std::string &showName,
            InstanceID id);
  void paint(juce::Graphics &g) override;
};

struct ChangedParameter {
  std::string name;
  double value;
};
class LCDInfo : public BaseComponent,
                public Events::Handler,
                public juce::AsyncUpdater {
public:
  LCDInfo(const std::string &name, const std::string &showName, InstanceID id);
  ~LCDInfo() override;
  void paint(juce::Graphics &g) override;
  void drawVU(juce::Graphics &g);
  void drawWave(juce::Graphics &g);
  void drawStereo(juce::Graphics &g);
  void drawChange(juce::Graphics &g);

  void handle(Events::Event *event) override;

  void handleAsyncUpdate() override;

protected:
  Theme::Theme *m_theme{nullptr};
  ChangedParameter m_changedParameter{};
  LCDInfoState m_state{LCDInfoState::ONCHANGE};
};

class LCDToolbar : public BaseComponent {
public:
  LCDToolbar(const std::string &name, const std::string &showName,
             InstanceID id);
  void paint(juce::Graphics &g) override;
};

} // namespace VeNo::GUI