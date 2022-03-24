#include "VUtils/Logging.h"
#include "VeNo/Core/Instance.h"

#include <Assets.h>
#include <VeNo/GUI/Components/Logo.h>
#include <utility>

namespace VeNo::GUI {

Logo::Logo(std::string name, std::string showName, size_t id)
    : BaseComponent(std::move(name), std::move(showName), id) {
  m_instance = VeNo::Core::Instance::get(m_id);
  createLogo();
  m_instance->eventHandler.addHandler("logo", this);
}
void Logo::paint(juce::Graphics &g) { m_instance->state.logo->draw(g, 1.0); }

void Logo::resized() {
  juce::RectanglePlacement placement = juce::RectanglePlacement::centred;
  juce::Rectangle<float> react{(float)pos.x, (float)pos.y, (float)pos.w,
                               (float)pos.h};
  auto transform = placement.getTransformToFit(
      m_instance->state.logo->getDrawableBounds(), react);
  m_instance->state.logo->setTransform(transform);
}

void Logo::handle(Events::Event *) { createLogo(); }
void Logo::createLogo() {
  VeNo::Theme::Theme *theme = VeNo::Core::Config::get().theme().get();
  m_instance->state.logo = juce::Drawable::createFromImageData(
      BinaryData::Logo_svg, BinaryData::Logo_svgSize);
  if (!m_instance->state.logo->replaceColour(
          juce::Colours::white, theme->getColor(Theme::Colors::logo)))
    ERR("Something went wrong swapping Color: WHITE");
  if (!m_instance->state.logo->replaceColour(
          juce::Colours::black, theme->getColor(Theme::Colors::logoAccent)))
    ERR("Something went wrong swapping Color: Black");
}
} // namespace VeNo::GUI