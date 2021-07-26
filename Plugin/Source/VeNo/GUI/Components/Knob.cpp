#include <VeNo/GUI/Components/Knob.h>

#include <VUtils/StringUtils.h>
#include <VeNo/Core/Config.h>
#include <VeNo/Core/Instance.h>
#include <VeNo/GUI/Events/LiveLabel.h>
#include <utility>

namespace VeNo::GUI
{
Knob::Knob (std::string name, std::string showName, size_t id)
    : BaseComponent (std::move (name), std::move (showName), id)
{
    m_labelPosition = LabelPosition::BOTTOM;
    m_slider = std::make_unique<juce::Slider>();
    // Styling Setters
    m_slider->setSliderStyle (getSliderStyle());
    m_slider->setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
    m_slider->setComponentID (std::to_string (id));
    // Enable Hover that shows the current Value
    m_slider->setPopupDisplayEnabled (false, true, getParentComponent(), 500);
    createLabel (m_showName, false);
    m_label->setEditable (true);
    m_label->addListener (this);
    m_label->setJustificationType (juce::Justification::centred);
    m_slider->setNumDecimalPlacesToDisplay (3);
    m_slider->addMouseListener(this, true);
    m_label->addMouseListener(this, true);

    // get instance and try to add Slider Attachments
    auto* instance = VeNo::Core::Instance::get (m_id);
    if (instance && instance->treeState)
    {
        auto state = instance->treeState;
        if (state->getParameter (m_name) != nullptr)
            m_attachment = std::make_unique<SliderAttachment> (*state, m_name, *m_slider);
        instance->state.components.addKnob (m_name, this);
    }
    addAndMakeVisible (*m_slider);
    addAndMakeVisible (*m_label);
}
void Knob::paint (juce::Graphics&)
{
}
void Knob::resized()
{
    auto pos = resize();
    m_slider->setBounds (pos.x, pos.y, pos.w, pos.h);
}

void Knob::labelTextChanged (juce::Label*)
{
    // We Ignore this but we have to implement this!
}
void Knob::editorShown (juce::Label*, juce::TextEditor& editor)
{
    char str[20];
    snprintf (str, 20, "%.3f", m_slider->getValue());
    editor.setJustification (juce::Justification::centred);
    editor.setText (str, false);
    if (m_liveLabel != nullptr)
        m_liveLabel->stopLive();
}
void Knob::editorHidden (juce::Label* label, juce::TextEditor&)
{
    auto text = label->getText().toStdString();
    double newValue = VUtils::StringUtils::toNumber (text, m_slider->getValue());
    m_slider->setValue (newValue);
}
Knob::~Knob()
{
    m_label->removeListener (this);
    m_attachment.reset (nullptr);
    delete m_liveLabel;
    auto* instance = VeNo::Core::Instance::get (m_id);
    if (instance)
        instance->state.components.removeKnob (m_name);
}
void Knob::enableLiveLabel (bool isSemi)
{
    if (m_liveLabel != nullptr)
        return;
    m_liveLabel = new VeNo::GUIEvents::LiveLabel (this, isSemi, showName());
    if (isSemi)
        m_liveLabel->sliderValueChanged (m_slider.get());
}
juce::Slider* Knob::slider()
{
    return m_slider.get();
}
juce::Label* Knob::label()
{
    return m_label.get();
}
juce::Slider::SliderStyle Knob::getSliderStyle()
{
    // look for user selection in config
    auto* props = VeNo::Core::Config::get().getProperties();
    int val = props->getIntValue ("sliderMode", -1);
    switch (val)
    {
        case 0:
            return juce::Slider::SliderStyle::Rotary;
        case 1:
            return juce::Slider::SliderStyle::RotaryHorizontalDrag;
        case 2:
            return juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag;
        default:
            return juce::Slider::SliderStyle::RotaryVerticalDrag;
    }
}
void Knob::openPopupMenu()
{
    DBGN ("Right Click > Open Modal over button")
    // Open a Modulation Window at Mouse Position for current knob parameter :D also quick adding new modulations here :P
}
void Knob::mouseDown (const juce::MouseEvent& event)
{
    if (event.mods.isRightButtonDown()) {
        openPopupMenu();
    }
}
} // namespace VeNo::GUI