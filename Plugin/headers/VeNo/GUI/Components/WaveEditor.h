#pragma once
#include <VeNo/GUI/Components/BaseComponent.h>

namespace VeNo::GUI {
class WaveEditor : public BaseComponent {
public:
  WaveEditor(std::string, const std::string &, size_t);
  void paint(juce::Graphics &) override;
  void resized() override;

  void setColorComponent(const GUIColorComponent& colorComponent);

protected:
  void afterParsing(Interpreter *) override;
  GUIColorComponent m_color{};
};
} // namespace VeNo::GUI