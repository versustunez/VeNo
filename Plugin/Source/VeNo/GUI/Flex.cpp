#include <VeNo/GUI/Components/BaseComponent.h>
#include <VeNo/GUI/Components/ComponentGroup.h>
#include <VeNo/GUI/Flex.h>
namespace VeNo::GUI {
Flex::Flex(std::unordered_map<std::string, std::string> &properties) {
  setAlignContent(properties["align-content"]);
  setAlignItems(properties["align-items"]);
  setJustifyContent(properties["justify-content"]);
  setDirection(properties["flex-direction"]);
  setWrap(properties["flex-wrap"]);
}
void Flex::setItems(Vector<Ref<BaseComponent>> &components,
                    Vector<Ref<ComponentGroup>> &groups) {
  m_flexBox.items.clear();
  for (auto &component : components) {
    auto &pos = component->pos;
    m_flexBox.items.add(
        juce::FlexItem(*component).withMinWidth((float)pos.w).withMinHeight((float)pos.h));
  }
  for (auto &component : groups) {
    auto &pos = component->position();
    m_flexBox.items.add(
        juce::FlexItem(*component).withMinWidth((float)pos.w).withMinHeight((float)pos.h));
  }
}
void Flex::perform(juce::Rectangle<float> rectangle) {
  m_flexBox.performLayout(rectangle);
}
void Flex::clear() { m_flexBox.items.clear(); }
void Flex::setJustifyContent(std::string &value) {
  if (value == "start")
    m_flexBox.justifyContent = FlexBox::JustifyContent::flexStart;
  else if (value == "end")
    m_flexBox.justifyContent = FlexBox::JustifyContent::flexEnd;
  else if (value == "center")
    m_flexBox.justifyContent = FlexBox::JustifyContent::center;
  else if (value == "space-around")
    m_flexBox.justifyContent = FlexBox::JustifyContent::spaceAround;
  else if (value == "space-between")
    m_flexBox.justifyContent = FlexBox::JustifyContent::spaceBetween;
}
void Flex::setWrap(std::string &value) {
  if (value == "no")
    m_flexBox.flexWrap = FlexBox::Wrap::noWrap;
  else if (value == "wrap")
    m_flexBox.flexWrap = FlexBox::Wrap::wrap;
  else if (value == "reverse")
    m_flexBox.flexWrap = FlexBox::Wrap::wrapReverse;
}
void Flex::setAlignContent(std::string &value) {
  if (value == "start")
    m_flexBox.alignContent = FlexBox::AlignContent::flexStart;
  else if (value == "end")
    m_flexBox.alignContent = FlexBox::AlignContent::flexEnd;
  else if (value == "center")
    m_flexBox.alignContent = FlexBox::AlignContent::center;
  else if (value == "space-around")
    m_flexBox.alignContent = FlexBox::AlignContent::spaceAround;
  else if (value == "space-between")
    m_flexBox.alignContent = FlexBox::AlignContent::spaceBetween;
  else if (value == "stretch")
    m_flexBox.alignContent = FlexBox::AlignContent::stretch;
}
void Flex::setAlignItems(std::string &value) {
  if (value == "start")
    m_flexBox.alignItems = FlexBox::AlignItems::flexStart;
  else if (value == "end")
    m_flexBox.alignItems = FlexBox::AlignItems::flexEnd;
  else if (value == "center")
    m_flexBox.alignItems = FlexBox::AlignItems::center;
  else if (value == "stretch")
    m_flexBox.alignItems = FlexBox::AlignItems::stretch;
}
void Flex::setDirection(std::string &value) {
  if (value == "row")
    m_flexBox.flexDirection = FlexBox::Direction::row;
  else if (value == "row-reverse")
    m_flexBox.flexDirection = FlexBox::Direction::rowReverse;
  else if (value == "column")
    m_flexBox.flexDirection = FlexBox::Direction::column;
  else if (value == "column-reverse")
    m_flexBox.flexDirection = FlexBox::Direction::columnReverse;
}
} // namespace VeNo::GUI
