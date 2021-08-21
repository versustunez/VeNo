#pragma once
#include <unordered_map>
#include <vector>
#include <string>
namespace VeNo::GUI
{
struct Position
{
    int x;
    int y;
    int w;
    int h;
};
struct GUIColorComponent
{
    bool hasColor = false;
    bool isPreColor = false;
    std::string preColor;
    uint8_t colors[4] = { 0, 0, 0, 255 };
    GUIColorComponent() = default;
    ~GUIColorComponent() = default;
};
struct GUIComponent
{
    std::string name;
    std::string parameter;
};

struct ImportItem
{
    std::string name;
    std::unordered_map<std::string, std::string> params;
};

struct GUIParseItem
{
    ~GUIParseItem();
    GUIParseItem() = default;
    GUIParseItem (GUIParseItem&) = delete;
    std::string name;
    std::unordered_map<std::string, std::string> properties;
    std::vector<GUIParseItem*> items; // we delete everything here ;)
    GUIParseItem* parent = nullptr;
    GUIComponent* component = nullptr;
    GUIColorComponent colorComponent;
    std::vector<ImportItem> imports;
    // we have x,y,w,h by default also a gui_parameter
    Position pos = { 0, 0, 1, 1 };
};
} // namespace VeNo::GUI