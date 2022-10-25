#pragma once
#include "../BaseComponent.h"
#include "ColorChangeComponent.h"

#include <VeNo/Core/Config.h>
#include <VeNo/GUI/Theme/Theme.h>

namespace VeNo::GUI {

class ConfigComponent : public BaseComponent,
                        public juce::Slider::Listener,
                        public juce::TextEditor::Listener {
public:
  ConfigComponent(const std::string &name, const std::string &showName,
                  InstanceID id);

  void init();
  void resized() override;
  void calculateSize(int width);

  void sliderValueChanged(juce::Slider *slider) override;
  void sliderDragEnded(juce::Slider *slider) override;
  void textEditorTextChanged(juce::TextEditor &editor) override;

protected:
  Core::Config *m_config{nullptr};
  Vector<Ref<BaseComponent>> m_components;
  Scope<juce::TextEditor> m_AuthorComponent;
  Flex m_flex;
};

} // namespace VeNo::GUI
