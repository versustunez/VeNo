#pragma once
#include "Structs.h"
#include "UIParser.h"

#include <VeNo/TypeDefs.h>
#include <optional>
#include <regex>

namespace VeNo::GUI {
class UIUtils {
public:
  enum class Direction { X = 0, Y };
  static int getValue(VString value, int parentValue);
  static int getOffsetFromParent(GUIParseItem *item, Direction direction);
  static VString getParameterReplaced(UIParser *parser, VString &input);
  static void setProperty(GUIParseItem *item, VString &name, VString &value);
  static ImportItem getImportParameters(UIParser *parser, VString &import);
  static void setColor(GUIParseItem *item, VString &value);

protected:
  static std::regex s_regEx;
};
} // namespace VeNo::GUI
