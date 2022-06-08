#pragma once

#include "../GUIFiles.h"
#include "Interpreter.h"
#include "UIParser.h"

namespace VeNo::GUI {
struct Initializer {
  Initializer() = default;
  ~Initializer();

  void parseMain(std::string &name);
  std::shared_ptr<UIParser> &createParser(std::string &name);

protected:
  static std::string getPreparedFilePath(const std::string &name);
  std::string getBinaryContent(const std::string &name);

public:
  GUIParseItem *get(const std::string &name);
  GUIParseItem *getOrCreate(const std::string &name);

protected:
  std::unordered_map<std::string, Ref<VeNo::GUI::UIParser>> m_guiParser;

};
} // namespace VeNo::GUI