#include "PresetManager.h"

#include "Instance.h"

#include <VeNo/Definitions.h>

namespace VeNo::Core {

// Static because I don't want to duplicate the same data over each instance...
PresetDatabase PresetManager::s_Presets{};
PresetManager::PresetManager(InstanceID id) : m_ID(id) {
  if (!s_Presets.Created)
    InitDatabase();
}

void PresetManager::Reset() {
  auto *instance = Instance::get(m_ID);
  if (instance->synthesizer) {
    instance->synthesizer->matrix().reset();
  }
  auto &parameters = instance->handler->GetParameters();
  auto *treeState = instance->treeState;
  for (auto &[key, value] : parameters) {
    auto *param = treeState->getParameter(key);
    param->setValueNotifyingHost(
        param->convertTo0to1((float)value->getDefaultValue()));
  }
  m_CurrentPresetData.FilePath = "";
  m_CurrentPresetData.Name = "Init";
  instance->eventHandler.triggerEvent("preset_changed",
                                      new Events::ChangeEvent);
  instance->eventHandler.triggerEvent("matrix-changed",
                                      new Events::ChangeEvent);
}

std::string PrepareFilePath(std::string name) {
  name.erase(std::remove_if(
                 name.begin(), name.end(),
                 [](char c) { return std::isspace(c) || !std::isalnum(c); }),
             name.end());
  return name;
}

void PresetManager::Save() {
  m_CurrentPresetData.Author = Config::get().properties()->asString(
      "user.author", juce::SystemStats::getFullUserName().toStdString());
  m_CurrentPresetData.FilePath = PrepareFilePath(m_CurrentPresetData.FilePath);
  if (m_CurrentPresetData.FilePath.empty()) {
    m_CurrentPresetData.FilePath = GetRandomFileName();
  }
  auto file = GetPresetFilePath(m_CurrentPresetData);
  bool needToAddToDatabase = !VUtils::FileHandler::fileExists(file);
  auto textFormat = juce::XmlElement::TextFormat().singleLine();
  auto data = GetCurrentData()->toString(textFormat).toStdString();
  if (!VUtils::FileHandler::writeFile(file, data)) {
    ERR("Cannot Save File: {}", file);
    return;
  }
  if (needToAddToDatabase) {
    s_Presets.Presets.push_back({m_CurrentPresetData.Name,
                                 m_CurrentPresetData.Author,
                                 m_CurrentPresetData.FilePath});

  } else {
    for (auto &preset : s_Presets.Presets) {
      if (preset.FilePath == m_CurrentPresetData.FilePath) {
        preset.Name = m_CurrentPresetData.Name;
        preset.Author = m_CurrentPresetData.Author;
        break;
      }
    }
  }
  SaveDatabase();
}

void PresetManager::Load(const std::string &path) {
  auto content = VUtils::FileHandler::readFile(path);
  if (!content.empty()) {
    auto data = juce::XmlDocument::parse(content);
    if (data)
      SetCurrentData(data);
  }
}

void PresetManager::LoadNext() {
  if (s_Presets.Presets.empty())
    return;
  if (m_CurrentIndex + 1 >= (int32_t)s_Presets.Presets.size())
    m_CurrentIndex = 0;
  else
    m_CurrentIndex++;
  LoadIndexPreset((size_t)m_CurrentIndex);
}

void PresetManager::LoadPrevious() {
  if (s_Presets.Presets.empty())
    return;
  if (m_CurrentIndex - 1 < 0)
    m_CurrentIndex = (int32_t)s_Presets.Presets.size() - 1;
  else
    m_CurrentIndex--;
  LoadIndexPreset((size_t)m_CurrentIndex);
}

void PresetManager::SetName(const std::string &name) {
  m_CurrentPresetData.Name = name;
}

Scope<juce::XmlElement> PresetManager::GetCurrentData() const {
  auto *instance = Instance::get(m_ID);
  auto state = instance->treeState->copyState();
  auto preset = new juce::XmlElement("VeNoPreset");
  preset->setAttribute("preset-name", m_CurrentPresetData.Name);
  preset->setAttribute("preset-author", m_CurrentPresetData.Author);
  preset->setAttribute("file-name", m_CurrentPresetData.FilePath);
  preset->setAttribute("version", VeNo_VERSION);
  if (instance->state.FXChain != nullptr) {
    preset->setAttribute("fx-series", instance->state.FXChain->Serialize());
  }
  preset->addChildElement(new juce::XmlElement(*state.createXml()));
  preset->addChildElement(instance->synthesizer->matrix().ToXml());
  auto xml = CreateScope<juce::XmlElement>(*preset);
  delete preset;
  return xml;
}

void PresetManager::SetCurrentData(const Scope<juce::XmlElement> &data) {
  auto *instance = Instance::get(m_ID);
  auto treeState = instance->treeState;
  auto params = data->getChildByName(treeState->state.getType());
  if (params != nullptr) {
    for (int i = 0; i < params->getNumChildElements(); ++i) {
      auto item = params->getChildElement(i);
      auto &src = item->getStringAttribute("id");
      auto amount = item->getDoubleAttribute("value");
      auto parameter = treeState->getParameter(src);
      if (parameter != nullptr)
        parameter->setValueNotifyingHost(
            parameter->convertTo0to1((float)amount));
    }
  }
  m_CurrentPresetData.Name =
      data->getStringAttribute("preset-name").toStdString();
  m_CurrentPresetData.Author =
      data->getStringAttribute("preset-author").toStdString();
  if (data->hasAttribute("fx-series")) {
    if (instance->state.FXChain) {
      instance->state.FXChain->Deserialize(
          data->getStringAttribute("fx-series").toStdString());
    } else {
      instance->state.PresetState["fx-series"] =
          data->getStringAttribute("fx-series").toStdString();
    }
  }
  auto fileName = data->getStringAttribute("file-name").toStdString();
  if (!fileName.empty())
    m_CurrentPresetData.FilePath = fileName;
  if (instance->synthesizer)
    instance->synthesizer->matrix().FromXML(data);
  instance->eventHandler.triggerEvent("preset_changed",
                                      new Events::ChangeEvent);
  instance->eventHandler.triggerEvent("matrix-changed",
                                      new Events::ChangeEvent);
}

void PresetManager::LoadIndexPreset(size_t index) {
  auto &element = s_Presets.Presets[index];
  auto filePath = GetPresetFilePath(element);
  DBGN("Loading Preset: {}", filePath);
  juce::File file{filePath};
  if (!file.exists())
    return;
  auto item = juce::XmlDocument::parse(file);
  if (item) {
    m_CurrentPresetData = s_Presets.Presets[index];
    SetCurrentData(item);
    m_CurrentIndex = index;
  }
}

Preset CreatePresetFromXML(const juce::XmlElement &data) {
  auto name = data.getStringAttribute("preset-name").toStdString();
  auto author = data.getStringAttribute("preset-author").toStdString();
  auto fileName = data.getStringAttribute("file-name").toStdString();
  return {std::move(name), std::move(author), std::move(fileName)};
}

void PresetManager::InitDatabase() {
  s_Presets.Created = true;
  auto &presetPath = Core::Config::get().presetPath;
  std::string dbFile{presetPath + ".venodb"};
  s_Presets.Presets.clear();
  if (VUtils::FileHandler::fileExists(dbFile)) {
    // Read that file instead ;)
    auto item = juce::XmlDocument::parse(juce::File(dbFile));
    if (item == nullptr)
      return;
    for (int i = 0; i < item->getNumChildElements(); i++) {
      auto *presetElement = item->getChildElement(i);
      auto name = presetElement->getStringAttribute("name").toStdString();
      auto author = presetElement->getStringAttribute("author").toStdString();
      auto path = presetElement->getStringAttribute("path").toStdString();
      Preset preset{name, author, path};
      // File does not exists... skip
      if (!VUtils::FileHandler::fileExists(GetPresetFilePath(preset))) {
        continue;
      }
      s_Presets.Presets.push_back(std::move(preset));
    }
  } else {
    InitDatabaseFromDisk();
  }
}

void PresetManager::SaveDatabase() {
  auto root = CreateScope<juce::XmlElement>("presets");
  for (auto &preset : s_Presets.Presets) {
    auto xmlPreset = new juce::XmlElement("VeNoPreset");
    xmlPreset->setAttribute("name", preset.Name);
    xmlPreset->setAttribute("author", preset.Author);
    xmlPreset->setAttribute("path", preset.FilePath);
    root->addChildElement(xmlPreset);
  }
  auto &presetPath = Core::Config::get().presetPath;
  std::string dbFile{presetPath + ".venodb"};
  root->writeTo(juce::File(dbFile));
}

void PresetManager::InitDatabaseFromDisk() {
  s_Presets.Presets.clear();
  auto &presetPath = Core::Config::get().presetPath;
  juce::File file{presetPath};
  auto files =
      file.findChildFiles(juce::File::TypesOfFileToFind::findFiles, false,
                          "*.vnp", juce::File::FollowSymlinks::no);

  for (auto &localFile : files) {
    auto item = juce::XmlDocument::parse(localFile);
    if (item == nullptr)
      continue;
    s_Presets.Presets.push_back(CreatePresetFromXML(*item));
  }
  SaveDatabase();
}

std::string PresetManager::GetRandomFileName() {
  const char charset[] = "0123456789"
                         "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                         "abcdefghijklmnopqrstuvwxyz";
  Utils::Random rng;
  rng.setRange(0, sizeof(charset) - 1);
  auto random_char = [&]() -> char { return charset[(int)rng.generate()]; };
  std::string str(24, 0);
  std::generate_n(str.begin(), 24, random_char);
  return str;
}

std::string PresetManager::GetPresetFilePath(const Preset &currentPreset) {
  auto &presetPath = Core::Config::get().presetPath;
  return presetPath + currentPreset.FilePath + ".vnp";
}
} // namespace VeNo::Core