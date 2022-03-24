#include <VUtils/Logging.h>
#include <VeNo/Core/Config.h>
#include <VeNo/GUI/GuiLang/Init.h>
#include <VeNo/Utils/ProfileMacros.h>

namespace VeNo::GUI {
Ref<UIParser> &Initializer::createParser(std::string &name) {
  if (m_guiParser.find(name) == m_guiParser.end()) {
#ifdef VENO_PROFILE
    VENO_PROFILE_SCOPE("Initializer::createParser > " + name);
#endif
    m_guiParser.emplace(name, CreateRef<UIParser>());
    auto &parser = m_guiParser[name];
    parser->setName(name);
    auto file = getPreparedFilePath(name);
    if (file.rfind("Bin::", 0) != 0)
      parser->loadFile(file);
    else
      parser->setContent(getBinaryContent(file));
  }
  return m_guiParser[name];
}
Initializer::~Initializer() { m_guiParser.clear(); }
void Initializer::parseMain(std::string &name) {
  VENO_PROFILE_FUNCTION();
  auto mainFB = "Bin::MainGUI";
  auto file = name == mainFB ? mainFB : getPreparedFilePath(name);
  m_guiParser.emplace("Main", CreateRef<UIParser>());
  auto& main = m_guiParser["Main"];
  if (VUtils::FileHandler::fileExists(file))
    main->loadFile(file);
  else
    main->setContent(Files::MainGui);
  main->setName(mainFB);
  main->parse();
}
std::string Initializer::getPreparedFilePath(const std::string &name) {
  if (name.find("Bin::", 0) == 0)
    return name;
  if (name.find(".gui") != std::string::npos)
    return name;
  auto &layoutPath = VeNo::Core::Config::get().layoutPath;
  return layoutPath + name + ".gui";
}

std::string Initializer::getBinaryContent(const std::string &name) {
  auto pos = name.rfind("Bin::", 0);
  auto binaryName = name;
  if (pos == 0)
    binaryName = binaryName.substr(5, binaryName.length());
  for (auto &preDefinedBinary : preDefinedBinaries) {
    if (binaryName == preDefinedBinary.name)
      return *preDefinedBinary.data;
  }
  WARN("UNKNOWN BINARY RETURN EMPTY!");
  return "";
}
GUIParseItem *Initializer::get(const std::string &name) {
  if (m_guiParser.find(name) != m_guiParser.end() &&
      m_guiParser[name] != nullptr)
    return m_guiParser[name]->rootItem();
  return nullptr;
}
GUIParseItem *Initializer::getOrCreate(const std::string &name) {
  if (m_guiParser.find(name) == m_guiParser.end() ||
      m_guiParser[name] == nullptr) {
#ifdef VENO_PROFILE
    VENO_PROFILE_SCOPE("Initializer::getOrCreate > " + name);
#endif
    auto fileName = getPreparedFilePath(name);
    bool isBin = name.rfind("Bin::", 0) == 0;
    bool fileExists = !isBin && VUtils::FileHandler::fileExists(fileName);
    m_guiParser.emplace(name, CreateRef<UIParser>());
    auto &item = m_guiParser[name];
    if (fileExists) {
      item->loadFile(fileName);
    } else {
      item->setContent(getBinaryContent(name));
    }
    item->parse();
  }
  return m_guiParser[name]->rootItem();
}
} // namespace VeNo::GUI