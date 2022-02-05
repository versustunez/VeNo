#pragma once
#include <VeNo/TypeDefs.h>
#include <string>
#include <unordered_map>
#include <vector>

namespace VeNo::GUI {
enum GUIDisplay { FIXED = 0, BLOCK };

struct Position {
  int x;
  int y;
  int w;
  int h;
};

struct Display {
  GUIDisplay x{GUIDisplay::FIXED};
  GUIDisplay y{GUIDisplay::FIXED};
};
struct GUIColorComponent {
  bool hasColor = false;
  bool isPreColor = false;
  std::string preColor;
  uint8_t colors[4] = {0, 0, 0, 255};
  GUIColorComponent() = default;
  ~GUIColorComponent() = default;
};
struct GUIComponent {
  std::string name;
  std::string parameter;
};

struct ImportItem {
  std::string name;
  Map<std::string, std::string> params;
};

struct GUIParseItem {
  ~GUIParseItem();
  GUIParseItem() = default;
  GUIParseItem(GUIParseItem &) = delete;
  std::string name;
  std::unordered_map<std::string, std::string> properties;
  std::vector<GUIParseItem *> items; // we delete everything here ;)
  GUIParseItem *parent = nullptr;
  GUIComponent *component = nullptr;
  GUIColorComponent colorComponent;
  Display display{};
  // we have x,y,w,h by default also a gui_parameter
  Position pos = {0, 0, 1, 1};

  bool has(const char *m_name) {
    return properties.find(m_name) != properties.end();
  }

  std::string &operator[](const char *_name);
};
} // namespace VeNo::GUI