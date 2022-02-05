#pragma once

#include "../GUIFiles.h"
#include "Interpreter.h"
#include "Parser.h"

namespace VeNo::GUI {
struct preDefinedBinary {
  std::string name;
  std::string *data;
};
struct Initializer {
  Initializer() = default;
  ~Initializer();

  void parseMain(std::string &name);
  std::shared_ptr<GUILangParser> &createParser(std::string &name);

protected:
  static std::string getPreparedFilePath(const std::string &name);
  std::string getBinaryContent(const std::string &name);

public:
  GUIParseItem *get(const std::string &name);
  GUIParseItem *getOrCreate(const std::string &name);

protected:
  std::unordered_map<std::string, Ref<VeNo::GUI::GUILangParser>> m_guiParser;
  preDefinedBinary preDefinedBinaries[5] = {{"Oscillator", &Files::OSC},
      {"LFO", &Files::EMPTY}, {"Filter", &Files::EMPTY}, {"LCD", &Files::EMPTY},
      {"WaveEditor", &Files::WaveEditor}};
};
} // namespace VeNo::GUI