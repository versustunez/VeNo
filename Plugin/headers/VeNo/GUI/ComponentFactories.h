#pragma once
#include "VeNo/GUI/GuiLang/Structs.h"
#include <VeNo/GUI/Components/BaseComponent.h>

namespace VeNo::GUI
{
class ComponentFactory
{
public:
    virtual std::shared_ptr<BaseComponent> create (GUIParseItem* item, const std::string& parameter, const std::string& name, size_t id);
};

class LabelComponentFactory : public ComponentFactory
{
public:
    std::shared_ptr<BaseComponent> create (
        GUIParseItem* item,
        const std::string& parameter,
        const std::string& name,
        size_t id) override;
};

class KnobComponentFactory : public ComponentFactory
{
public:
    std::shared_ptr<BaseComponent> create (
        GUIParseItem* item,
        const std::string& parameter,
        const std::string& name,
        size_t id) override;
};
} // namespace VeNo::GUI