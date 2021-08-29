#pragma once

#include <JuceHeader.h>
#include <VeNo/Definitions.h>
#include <memory>
#include <string>

namespace VeNo::Core
{
enum ParameterTypes
{
    Integer = 0,
    Float,
    Boolean
};

class Parameter
{
protected:
    size_t m_id {};
    std::string m_name;
    std::string m_showName;
    double m_max {};
    double m_min {};
    double m_value {};

public:
    Parameter (std::string name, std::string showName, double min, double max, double value, size_t id);
    virtual ~Parameter() = default;
    bool getBool();
    int getInt();
    double getValue();
    virtual void setValue (double value);
    double getMin();
    double getMax();
    std::string& getName();
    std::string& getShowName();
    std::unique_ptr<juce::RangedAudioParameter> createParameter (ParameterTypes type);
};

class ModulateParameter : public Parameter
{
protected:
    double m_values[MAX_VOICES] {};
    double m_matrix[MAX_VOICES] {};
    double m_matrixPos = 0.0;
    double m_normalised = 0.0;
    double m_preMinMax = 0;

public:
    ModulateParameter (std::string name, std::string showName, double min, double max, double value, size_t id);
    double getVoice (int voice);
    int getVoiceInt (int voice);
    void setVoiceValue (int voice, double value);
    void setValue (double value) override;
    void addValue (double value);
    void addValueVoice (int voice, double value);
    void reset();
};
} // namespace VeNo::Core