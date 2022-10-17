#include <VUtils/Logging.h>
#include <VeNo/Core/Config.h>
#include <VeNo/GUI/GuiLang/Init.h>

namespace VeNo::GUI {
Ref<UIParser> &Initializer::createParser(std::string &name) {
  if (m_guiParser.find(name) == m_guiParser.end()) {
    m_guiParser.emplace(name, CreateRef<UIParser>());
    auto &parser = m_guiParser[name];
    parser->setName(name);
    auto file = getPreparedFilePath(name);
    if (file.rfind("Bin::", 0) != 0) {
      parser->loadFile(file);
    } else
      parser->setContent(getBinaryContent(file));
  }
  return m_guiParser[name];
}

Initializer::~Initializer() { m_guiParser.clear(); }

void Initializer::parseMain(std::string &name) {
  auto mainFB = "Bin::MainGui";
  auto file = getPreparedFilePath(name);
  DBGN("Using {} as MainGui File", file);
  m_guiParser.emplace("Main", CreateRef<UIParser>());
  auto &main = m_guiParser["Main"];
  if (file.rfind("Bin::") != 0 && VUtils::FileHandler::fileExists(file))
    main->loadFile(file);
  else
    main->setContent(getBinaryContent(file));
  main->setName(mainFB);
  main->parse();
}

std::string Initializer::getPreparedFilePath(const std::string &name) {
  if (name.find("Bin::", 0) == 0)
    return name;
  auto &layoutPath = VeNo::Core::Config::get().layoutPath;
  auto out = layoutPath + name + ".vui";
  if (VUtils::FileHandler::fileExists(out)) {
    return out;
  } else {
    return "Bin::" + name;
  }
}

std::string Initializer::getBinaryContent(const std::string &name) {
  auto pos = name.rfind("Bin::", 0);
  auto binaryName = name;
  if (pos == 0)
    binaryName = binaryName.substr(5, binaryName.length());

  for (auto &preDefinedBinary : Files::preDefinedBinaries) {
    if (binaryName == preDefinedBinary.name)
      return preDefinedBinary.data;
  }
  WARN("UNKNOWN BINARY \"{}\" > RETURN EMPTY!", binaryName);
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