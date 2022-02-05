#pragma once

#include "Structs.h"
#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>

namespace VeNo::GUI {
class GUILangParser {
public:
  GUILangParser();
  ~GUILangParser();
  GUILangParser(GUILangParser &) = delete;
  explicit GUILangParser(std::string file, bool isFile);
  void load();
  void setContent(const std::string &content);
  void parse(bool headOnly);
  static ImportItem getImportParameters(std::string &import);
  static void setProperty(
      GUIParseItem *item, std::string &name, std::string &value);
  std::string m_file;
  std::vector<std::string> m_lines;
  GUIParseItem *rootItem{};
  std::unordered_map<std::string, std::string> m_info;
  bool m_isInvalid = false;
  bool shouldCrash = false;
  bool m_isFile = true;
  int crashedOnLine = -1;
  std::string parserName = "unknown_file";
  static std::string preColors[5];

  Map<std::string, std::string> parameters;

protected:
  // Yeah Regex! :(
  std::regex regex{"\\{(.*?)\\}",
      std::regex_constants::ECMAScript | std::regex_constants::icase};
  static int getValue(std::string &value, int parentVal);
  static int getOffsetFromParent(GUIParseItem *pItem, bool b);
  std::string getParameterReplaced(std::string input);
};
} // namespace VeNo::GUI
