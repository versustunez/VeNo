#include <VeNo/GUI/Components/WaveEditor.h>

#include <VeNo/Window/WaveEditor.h>
#include <utility>

namespace VeNo::GUI {
WaveEditor::WaveEditor(std::string name, const std::string &showName, size_t id)
    : BaseComponent(std::move(name), showName, id) {}
void WaveEditor::afterParsing(Interpreter *) {
  // we need to get the items for position stuff
}
void WaveEditor::paint(juce::Graphics &g) {}
void WaveEditor::resized() {}
} // namespace VeNo::GUI
  // namespace VeNo::GUI