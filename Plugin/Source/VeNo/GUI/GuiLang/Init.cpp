#include <VUtils/Logging.h>
#include <VeNo/Core/Config.h>
#include <VeNo/GUI/GuiLang/Init.h>
#include <VeNo/Utils/ProfileMacros.h>

namespace VeNo::GUI {
Ref<GUILangParser> &Initializer::createParser(std::string &name) {
  if (m_guiParser.find(name) == m_guiParser.end()) {
#ifdef VENO_PROFILE
    VENO_PROFILE_SCOPE("Initializer::createParser > " + name);
#endif
    m_guiParser.emplace(
        name, CreateRef<GUILangParser>(getPreparedFilePath(name), true));
    auto &parser = m_guiParser[name];
    if (parser->m_file.rfind("Bin::", 0) != 0)
      parser->load();
    else
      parser->setContent(getBinaryContent(parser->m_file));
  }
  return m_guiParser[name];
}
Initializer::~Initializer() { m_guiParser.clear(); }
void Initializer::parseMain(std::string &name) {
  VENO_PROFILE_FUNCTION();
  auto mainFB = "Bin::MainGUI";
  auto file = name == mainFB ? mainFB : getPreparedFilePath(name);
  if (name != mainFB && VUtils::FileHandler::fileExists(file))
    m_guiParser.emplace("Main", CreateRef<GUILangParser>(file, true));
  else
    m_guiParser.emplace("Main",
                        CreateRef<GUILangParser>(Files::MainGui, false));

  auto &main = m_guiParser["Main"];
  main->parserName = mainFB;
  main->load();
  main->parse(false);
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
    return m_guiParser[name]->rootItem;
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
    m_guiParser.emplace(name, CreateRef<GUILangParser>(name, fileExists));
    auto &item = m_guiParser[name];
    std::string content;
    if (fileExists)
      content = VUtils::FileHandler::readFile(fileName);
    else
      content = getBinaryContent(name);
    item->setContent(content);
    item->parse(false);
  }
  return m_guiParser[name]->rootItem;
}
} // namespace VeNo::GUI