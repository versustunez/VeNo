#include "PresetManager.h"

#include "Instance.h"
#include "vendors/NameGen/NameGen.h"

namespace VeNo::Core {
PresetManager::PresetManager(InstanceID id) : m_ID(id) {
  NameGen::Generator generator("s~Vi");
  m_PresetName = "Init";
  m_CurrentPreset = generator.toString() + ".vnp";
}

void PresetManager::Reset() {}
void PresetManager::Save() {
  auto &presetPath = Core::Config::get().presetPath;
  if (m_CurrentPreset.empty())
    m_CurrentPreset = m_PresetName + ".vnp";
  std::string file = presetPath + m_CurrentPreset;
  auto textFormat = juce::XmlElement::TextFormat().singleLine();
  auto data = GetCurrentData()->toString(textFormat).toStdString();
  if (!VUtils::FileHandler::writeFile(file, data)) {
    ERR("Cannot Save File: {}", file);
  }
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
  // @TODO
}
void PresetManager::LoadPrevious() {
  // @TODO
}
void PresetManager::SetName(const std::string &name) { m_PresetName = name; }
Scope<juce::XmlElement> PresetManager::GetCurrentData() {
  auto *instance = Instance::get(m_ID);
  auto state = instance->treeState->copyState();
  auto preset = new juce::XmlElement("VeNoPreset");
  preset->setAttribute("preset-name", m_PresetName);
  preset->setAttribute("file-name", m_CurrentPreset);
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
        parameter->setValueNotifyingHost(parameter->convertTo0to1((float)amount));
    }
  }
  m_PresetName = data->getStringAttribute("preset-name").toStdString();
  if (data->hasAttribute("fx-series")) {
    if (instance->state.FXChain) {
      instance->state.FXChain->Deserialize(data->getStringAttribute("fx-series").toStdString());
    } else {
      instance->state.PresetState["fx-series"] = data->getStringAttribute("fx-series").toStdString();
    }
  }
  auto fileName = data->getStringAttribute("file-name").toStdString();
  if (!fileName.empty())
    m_CurrentPreset = fileName;
  if (instance->synthesizer)
    instance->synthesizer->matrix().FromXML(data);
  instance->eventHandler.triggerEvent("preset_changed",
                                      new Events::ChangeEvent);
}
} // namespace VeNo::Core