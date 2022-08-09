#pragma once

#include "Structs.h"

#include <VeNo/TypeDefs.h>
#include <stack>

namespace VeNo::GUI {
struct UIParserLoop {
  size_t beginningLine{0};
  int times{-1};
  int offset{0};
  int done{0};
  size_t size{0};
  VString itVar{};
  VString itVarOffset{};
  VString userIteratorVar{};
};
class UIParser {
public:
  void setContent(const VString &content);
  void loadFile(const VString &filePath);
  void parse();

  GUIParseItem *rootItem() { return m_rootItem; }
  auto &parameters() { return m_parameters; }
  auto &name() { return m_name; }
  void setName(const VString &name) { m_name = name; }

public:
  // validation allow to run the parser with error reporting
  void enableValidation() { m_shouldCrash = true; }

protected:
  GUIParseItem *createNew(GUIParseItem *previous);
  GUIParseItem *createGroup(GUIParseItem *previous, VString &line);
  GUIParseItem *createComponent(GUIParseItem *previous, VString &line);
  void importLine(GUIParseItem *previous, VString &line);
  void parseInfo(VString &line);
  static UIParserLoop getLoop(VString &line, size_t lineNumber);

protected:
  VString m_content{};
  Vector<VString> m_lines;
  bool m_isFile{false};
  bool m_shouldCrash{false};
  size_t m_erroredLine{0};

protected:
  GUIParseItem *m_rootItem;
  Map<VString, VString> m_info;
  VString m_name{"$Root"};
  Map<VString, VString> m_parameters;
  std::stack<GUIParseItem *> m_stack{};
  std::stack<UIParserLoop> m_loopStack{};
};
} // namespace VeNo::GUI