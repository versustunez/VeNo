#pragma once
#include "VeNo/Core/Instance.h"

#include <VeNo/GUI/Components/BaseComponent.h>
#include <VeNo/TypeDefs.h>

namespace VeNo::GUI {
class Logo : public BaseComponent, public Events::Handler {
public:
  Logo(std::string name, std::string showName, InstanceID id);
  ~Logo() override { m_instance->eventHandler.removeHandler("logo"); }

  void paint(juce::Graphics &g) override;
  void resized() override;

  void handle(Events::Event *event) override;

protected:
  void createLogo();

protected:
  Core::Instance *m_instance;
};
} // namespace VeNo::GUI