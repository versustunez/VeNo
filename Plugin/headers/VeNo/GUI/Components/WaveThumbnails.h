#pragma once
#include "BaseComponent.h"
#include "Button.h"

#include <VeNo/GUI/Flex.h>

namespace VeNo::GUI {
class WaveThumbnails : public BaseComponent {
public:
  WaveThumbnails(const std::string &name, const std::string &showName,
                 size_t id);

  void resized() override;
  void updateSize();
  void setupFlex();

protected:
  Vector<Ref<BaseComponent>> m_components;
  Ref<Button> m_addButton;
  Flex m_flex;
};
} // namespace VeNo::GUI