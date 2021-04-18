#include <VeNo/GUI/Components/Knob.h>

#include <VUtils/StringUtils.h>
#include <VeNo/Core/Instance.h>
#include <VeNo/GUI/GUIUtils.h>
#include <utility>

namespace VeNo::GUI
{
Knob::Knob (std::string name, std::string showName, size_t id)
    : BaseComponent (std::move (name), std::move (showName), id)
{
    m_labelPosition = LabelPosition::BOTTOM;
    m_slider = std::make_unique<juce::Slider>();
    // Styling Setters
    m_slider->setSliderStyle (juce::Slider::SliderStyle::RotaryVerticalDrag);
    m_slider->setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);

    // Enable Hover that shows the current Value
    m_slider->setPopupDisplayEnabled (true, true, getParentComponent(), 500);
    m_slider->setPopupMenuEnabled (true);
    createLabel (m_showName, false);
    m_label->setEditable (true);
    m_label->addListener (this);
    m_label->setJustificationType (juce::Justification::centred);

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
    snprintf (str, 20, "%.2f", m_slider->getValue());
    editor.setJustification (juce::Justification::centred);
    editor.setText (str, false);
}
void Knob::editorHidden (juce::Label* label, juce::TextEditor&)
{
    auto text = label->getText().toStdString();
    double newValue = VUtils::StringUtils::stringToDouble (text, m_slider->getValue());
    m_slider->setValue (newValue);
    if (! m_liveLabel)
        setText (m_showName);
}
Knob::~Knob()
{
    m_label->removeListener (this);
    m_slider->removeListener (this);
    m_attachment.reset (nullptr);
    m_slider.reset();
    auto* instance = VeNo::Core::Instance::get (m_id);
    if (instance)
        instance->state.components.removeKnob (m_name);
}
void Knob::enableLiveLabel()
{
    m_liveLabel = true;
    m_slider->addListener (this);
    sliderValueChanged (m_slider.get());
}
void Knob::sliderValueChanged (juce::Slider*)
{
    char str[20];
    snprintf (str, 20, "%.2f", m_slider->getValue());
    setText (str);
}
} // namespace VeNo::GUI