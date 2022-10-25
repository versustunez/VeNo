#pragma once

#include "VeNo/TypeDefs.h"

#include <JuceHeader.h>

namespace VeNo::Core {

struct Preset {
  std::string Name;
  std::string Author;
  std::string FilePath;
};

struct PresetDatabase {
  Vector<Preset> Presets;
  bool Created{false};
};

class PresetManager {
public:
  explicit PresetManager(InstanceID id);
  ~PresetManager() = default;
  void Reset();
  void Save();
  void Load(const std::string &path);
  void LoadNext();
  void LoadPrevious();
  void SetName(const std::string &name);
  std::string &GetName() { return m_CurrentPresetData.Name; }
  Scope<juce::XmlElement> GetCurrentData() const;
  Preset& GetCurrentPreset() { return m_CurrentPresetData; }
  void SetCurrentData(const Scope<juce::XmlElement> &data);
  Vector<Preset>& GetPresets() { return s_Presets.Presets; }
  static PresetDatabase& GetPresetDatabase() { return s_Presets; }
  static void SaveDatabase();
  void LoadIndexPreset(size_t index);
  static void InitDatabaseFromDisk();

  static std::string GetRandomFileName();


protected:
  static void InitDatabase();
  static std::string GetPresetFilePath(const Preset& currentPreset);
protected:
  InstanceID m_ID{};
  static PresetDatabase s_Presets;
  Preset m_CurrentPresetData{"Init", "VeNo", ""};
  int32_t m_CurrentIndex{0};
};

} // namespace VeNo::Core
