#pragma once
#include "VeNo/Utils/WavePointUtils.h"

#include <VeNo/GUI/Components/BaseComponent.h>
#include <VeNo/GUI/Components/Label.h>
#include <VeNo/GUI/Components/WaveThumbnails.h>
#include <VeNo/Sound/Tables/WaveLib.h>

namespace VeNo::GUI {
class WaveEditor : public BaseComponent, public Events::Handler {
public:
  WaveEditor(std::string, const std::string &, InstanceID);
  void paint(juce::Graphics &) override;
  void paintOverChildren(juce::Graphics &g) override;
  void resized() override;

  void setColorComponent(const GUIColorComponent &colorComponent);
  void mouseDrag(const juce::MouseEvent &event) override;
  void mouseDown(const juce::MouseEvent &event) override;
  void mouseUp(const juce::MouseEvent &event) override;
  void mouseDoubleClick(const juce::MouseEvent &event) override;
  void handle(Events::Event *) override;

protected:
  void drawDots(juce::Graphics& graphics);
  void triggerEnd(const juce::MouseEvent& event);
protected:
  void afterParsing(Interpreter *) override;
  GUIColorComponent m_color{};
  WaveThumbnails *m_thumbnails{nullptr};
  Label *m_xPointComponent{nullptr};
  Label *m_yPointComponent{nullptr};
  Audio::WaveTableLib* m_lib{};
  size_t m_currentWave{0};
  Ref<WaveThumbnail> m_currentThumbnail{};
  Utils::WavePoints m_currentPoint{nullptr};
  bool m_changed{false};
};
} // namespace VeNo::GUI