#pragma once
#include "../BaseComponent.h"
#include "ColorComponent.h"

#include <VeNo/Core/Config.h>
#include <VeNo/GUI/Theme/Theme.h>

namespace VeNo::GUI {

class ColorChangeComponent : public BaseComponent, public ColorChangeListener {
public:
  ColorChangeComponent(const std::string &name, const std::string &showName,
                       InstanceID id);

  void init();
  void paint(juce::Graphics &g) override;
  void setColorIndex(Theme::Colors idx) { m_color = idx; }
  void resized() override;
  void mouseDown(const juce::MouseEvent &event) override;
  void HandleColorChange(const juce::Colour &color) override;

protected:
  Core::Config *m_config{nullptr};
  Theme::Theme *m_theme{nullptr};
  Theme::Colors m_color{Theme::Colors::unknown};
  Ref<Label> m_label{};
};

} // namespace VeNo::GUI
