#pragma once

#include "VeNo/TypeDefs.h"
#include <JuceHeader.h>

namespace VeNo::Core {

class PresetManager {
public:
  explicit PresetManager(InstanceID id);
  ~PresetManager() = default;
  void Reset();
  void Save();
  void Load(const std::string& path);
  void LoadNext();
  void LoadPrevious();
  void SetName(const std::string& name);
  std::string& GetName() { return m_PresetName; }
  Scope<juce::XmlElement> GetCurrentData();
  void SetCurrentData(const Scope<juce::XmlElement>& data);
protected:
  InstanceID m_ID{};
  std::string m_PresetName{};
  std::string m_CurrentPreset{};
};

} // namespace VeNo::Core
