#pragma once

#include "BaseComponent.h"
#include "Button.h"
#include "Knob.h"

namespace VeNo::GUI {

class BigMatrixItem : public BaseComponent,
                      public Events::Handler,
                      public juce::Slider::Listener {
public:
  BigMatrixItem(std::string matrixKey, InstanceID id);
  ~BigMatrixItem() override;
  void paint(juce::Graphics &g) override;
  void resized() override;
  void handle(Events::Event *event) override;
  void sliderValueChanged(juce::Slider *slider) override;

protected:
  std::string m_MatrixKey{};
  Scope<Knob> m_AmountKnob{};
  Scope<Button> m_ActionButton{};
};

class BigMatrix : public BaseComponent, public Events::Handler {
public:
  BigMatrix(const std::string &name, const std::string &showName,
            InstanceID id);
  ~BigMatrix() override;
  void handle(Events::Event *event) override;

  void UpdateSize();

protected:
  Map<std::string, Scope<BigMatrixItem>> m_Items{};
  void CreateList();
};
} // namespace VeNo::GUI
