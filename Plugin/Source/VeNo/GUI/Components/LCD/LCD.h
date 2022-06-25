#pragma once
#include "../BaseComponent.h"
#include "LCDSubComponents.h"
#include "VeNo/GUI/Theme/Theme.h"

namespace VeNo::GUI {
class LCDComponent : public BaseComponent {
public:
  LCDComponent(const std::string &name, const std::string &showName,
               InstanceID id);

  void resized() override;
  void paint(juce::Graphics &g) override;

  void init();

protected:
  Ref<LCDHeader> m_header{};
  Ref<LCDInfo> m_info{};
  Ref<LCDToolbar> m_toolbar{};
  Theme::Theme *m_theme{nullptr};
};
} // namespace VeNo::GUI