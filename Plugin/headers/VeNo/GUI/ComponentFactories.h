#pragma once
#include "VeNo/GUI/GuiLang/Structs.h"
#include <VeNo/GUI/Components/BaseComponent.h>

namespace VeNo::GUI
{
class Interpreter;
class ComponentFactory
{
public:
    virtual std::shared_ptr<BaseComponent> create (GUIParseItem* item, const std::string& parameter, const std::string& name, size_t id, Interpreter* interpreter);
    static void setSelector (BaseComponent* component, GUIParseItem* item);
    static void doBase(BaseComponent*, GUIParseItem*, Interpreter*);
    static bool contains(const char* name, GUIParseItem* item);
};

class LabelComponentFactory : public ComponentFactory
{
public:
    std::shared_ptr<BaseComponent> create (
        GUIParseItem* item,
        const std::string& parameter,
        const std::string& name,
        size_t id,
        Interpreter* interpreter) override;
};

class KnobComponentFactory : public ComponentFactory
{
public:
    std::shared_ptr<BaseComponent> create (
        GUIParseItem* item,
        const std::string& parameter,
        const std::string& name,
        size_t id,
        Interpreter* interpreter) override;
};

class WaveEditorFactory : public ComponentFactory
{
public:
    std::shared_ptr<BaseComponent> create (
        GUIParseItem* item,
        const std::string& parameter,
        const std::string& name,
        size_t id,
        Interpreter* interpreter) override;
};

class ToggleButtonFactory : public ComponentFactory
{
public:
    std::shared_ptr<BaseComponent> create (
        GUIParseItem* item,
        const std::string& parameter,
        const std::string& name,
        size_t id,
        Interpreter* interpreter) override;
};

class ButtonFactory : public ComponentFactory
{
public:
    std::shared_ptr<BaseComponent> create (
        GUIParseItem* item,
        const std::string& parameter,
        const std::string& name,
        size_t id,
        Interpreter* interpreter) override;
};

} // namespace VeNo::GUI