#pragma once
#include "../BaseComponent.h"

#include <VeNo/Core/Config.h>
#include <VeNo/GUI/Theme/Theme.h>

namespace VeNo::GUI {

class ColorChangeComponent : public BaseComponent, public juce::ChangeListener {
public:
  ColorChangeComponent(const std::string &name, const std::string &showName,
                       InstanceID id);

  void init();
  void paint(juce::Graphics &g) override;
  void setColorIndex(Theme::Colors idx) {
    m_color = idx;
    m_select->setCurrentColour(m_theme->getColor(idx), juce::NotificationType::dontSendNotification);
  }
  void resized() override;
  juce::Colour colour() { return m_select->getCurrentColour(); }

  void changeListenerCallback(juce::ChangeBroadcaster *source) override;

protected:
  Core::Config *m_config{nullptr};
  Theme::Theme *m_theme{nullptr};
  Theme::Colors m_color{Theme::Colors::unknown};
  Scope<juce::ColourSelector> m_select{};
  Ref<Label> m_label{};
};

} // namespace VeNo::GUI
