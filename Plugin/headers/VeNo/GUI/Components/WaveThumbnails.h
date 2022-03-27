#pragma once
#include "BaseComponent.h"
#include "Button.h"

#include <VeNo/Events/WaveEvents.h>
#include <VeNo/GUI/Flex.h>

namespace VeNo::GUI {
class WaveThumbnail : public BaseComponent {
public:
  WaveThumbnail(const std::string &name, const std::string &showName, size_t id)
      : BaseComponent(name, showName, id){};
  void mouseDown(const juce::MouseEvent &event) override;
  void paint(juce::Graphics &g) override;
  void generateWaveForm();
  size_t thumbId{};
  Audio::WaveTableLib *lib{nullptr};
  bool drawBackground{true};
  bool isCurrent{false};
  void resized() override;

protected:
  juce::Path m_path;
};
class WaveThumbnails : public BaseComponent, public Events::Handler {
public:
  WaveThumbnails(const std::string &name, const std::string &showName,
                 size_t id);

  void resized() override;
  void updateSize();
  void setupFlex();
  void handle(Events::Event *event) override;
  void mouseUp(const juce::MouseEvent &event) override;
  void createThumbnails();
  void regenerateCurrent();
  [[nodiscard]] size_t currentWave() const { return m_currentWave; }

  void handleWaveThumb(WaveThumbnail *thumb, GUI::WaveEditType editType);

protected:
  void createThumbnail(size_t index, Audio::WaveTableLib *);
  size_t m_currentWave{};
  Vector<Ref<BaseComponent>> m_components{};
  Ref<Button> m_addButton{};
  Flex m_flex{};
  WaveThumbnail *m_currentThumbnail{nullptr};
};

class WaveForm : public BaseComponent, public Events::Handler {
public:
  WaveForm(const std::string &name, const std::string &showName,
                 size_t id);
  void setIsOscillator(bool isOsc) { m_isOscillator = isOsc; }
  void setWaveId(size_t idx) { m_waveId = idx; }
  void init();
  void resized() override;

  void handle(Events::Event *event) override;

protected:
  bool m_isOscillator{false};
  size_t m_waveId{0};
  Ref<WaveThumbnail> m_thumbnail;
  bool m_isInit{false};
};
} // namespace VeNo::GUI