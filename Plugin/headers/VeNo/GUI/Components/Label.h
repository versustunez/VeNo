#pragma once
#include "BaseComponent.h"
#include <JuceHeader.h>

namespace VeNo::GUI {
class LabelEventListener;
class Label : public BaseComponent {
public:
  Label(std::string name, std::string showName, size_t id);
  void setText(std::string text);
  void setAlign(const std::string& align);
  void resized() override;

  void addListenEvent(const std::string& event);

protected:
  std::string m_text;
  Scope<juce::Label> m_label;
  Scope<LabelEventListener> m_eventListener{nullptr};
};

class LabelEventListener: public Events::Handler {
public:
  LabelEventListener(std::string  name, Label* label);
  ~LabelEventListener() override;
  void handle(Events::Event *event) override;
protected:
  Label* m_label;
  std::string m_name;
};
} // namespace VeNo::GUI