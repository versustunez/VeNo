#include "VeNo/Core/Config.h"

#include <VeNo/GUI/Components/WaveEditor.h>
#include <VeNo/Window/WaveEditor.h>
#include <utility>

namespace VeNo::GUI {
WaveEditor::WaveEditor(std::string name, const std::string &showName, size_t id)
    : BaseComponent(std::move(name), showName, id) {

}

void WaveEditor::afterParsing(Interpreter *) {
  // we need to get the items for position stuff
}

void WaveEditor::paint(juce::Graphics &g) {
  if(m_color.hasColor) {
    auto *theme = Core::Config::get().theme().get();
    if (m_color.isPreColor)
      g.setColour(theme->getColor(m_color.preColor));
    else
      g.setColour(juce::Colour(m_color.colors[0], m_color.colors[1],
                               m_color.colors[2], m_color.colors[3]));
    g.fillAll();
  }
}

void WaveEditor::resized() {

}

void WaveEditor::setColorComponent(const GUIColorComponent& colorComponent) {
  m_color = colorComponent;
}

} // namespace VeNo::GUI