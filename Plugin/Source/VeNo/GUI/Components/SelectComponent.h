#pragma once

#include "BaseComponent.h"

#include <JuceHeader.h>
#include <VeNo/TypeDefs.h>

namespace VeNo::GUI {
typedef juce::AudioProcessorValueTreeState::ComboBoxAttachment
    ComboBoxAttachment;
class Select : public BaseComponent {
public:
  Select(const std::string &name, const std::string &showName, InstanceID id);
  void resized() override;
  void addItem(const std::string &value);
  void createAttachment();
  void setupPreset(const VString &preset);

protected:
  Ref<juce::ComboBox> m_comboBox;
  int m_lastAdded{1};
  Scope<ComboBoxAttachment> m_attachment;
};
} // namespace VeNo::GUI