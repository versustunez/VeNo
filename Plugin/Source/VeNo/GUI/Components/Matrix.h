#pragma once

#include "BaseComponent.h"
#include "Button.h"
#include "Knob.h"
#include "VeNo/Sound/Synth/Matrix.h"

namespace VeNo::Core {
class Instance;
}
namespace VeNo::GUI {
class MatrixItemComponent : public VComponent,
                            public juce::Button::Listener,
                            public juce::Slider::Listener {
public:
  MatrixItemComponent(const std::string &name, const std::string &modulator,
                      InstanceID id);
  void buttonClicked(juce::Button *button) override;
  void sliderValueChanged(juce::Slider *slider) override;
  void paint(juce::Graphics &g) override;
  void resized() override;

protected:
  InstanceID m_id;
  VString m_Key;
  VString m_Modulator;
  VString m_Name;
  Ref<Knob> m_AmountKnob;
  Ref<juce::Label> m_MatrixLabel;
  Ref<Button> m_AddRemoveButton;
};
class MatrixComponent : public BaseComponent {
public:
  MatrixComponent(std::string name, InstanceID id);
  void resized() override;
  void paint(juce::Graphics &g) override;

private:
  VString m_componentName;

protected:
  Core::Instance *m_Instance;
  Audio::Matrix *m_Matrix;
  Vector<Ref<MatrixItemComponent>> m_Items;
  friend MatrixItemComponent;
};
} // namespace VeNo::GUI
