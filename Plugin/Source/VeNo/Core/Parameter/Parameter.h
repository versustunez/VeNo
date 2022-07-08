#pragma once

#include <JuceHeader.h>
#include <VeNo/Definitions.h>
#include <VeNo/TypeDefs.h>
#include <memory>
#include <string>

namespace VeNo::Core {
enum ParameterTypes { Integer = 0, Float, Boolean };

class Parameter : public juce::AudioProcessorParameter::Listener {
protected:
  InstanceID m_id{};
  std::string m_name;
  std::string m_showName;
  double m_max{};
  double m_min{};
  double m_value{};
  int m_precision{0};

public:
  Parameter(std::string name, std::string showName, double min, double max,
            double value, InstanceID id);
  ~Parameter() override = default;
  bool getBool();
  int getInt();
  double getValue();
  virtual void setValue(double value);
  double getMin();
  double getMax();
  int precision();
  void calculatePrecision(float interval);
  std::string &getName();
  std::string &getShowName();
  Scope<juce::RangedAudioParameter> createParameter(ParameterTypes type);
  void parameterValueChanged(int parameterIndex, float newValue) override;
  void parameterGestureChanged(int parameterIndex,
                               bool gestureIsStarting) override;

  explicit operator bool() const { return m_value < 0.5; }
  explicit operator int() const { return (int)m_value; }
  explicit operator double() const { return m_value; }
};

class ModulateParameter : public Parameter {
protected:
  double m_values[MAX_VOICES]{};
  double m_matrix[MAX_VOICES]{};
  double m_matrixPos = 0.0;
  double m_normalised = 0.0;
  double m_preMinMax = 0;

public:
  ModulateParameter(std::string name, std::string showName, double min,
                    double max, double value, InstanceID id);
  double getVoice(int voice);
  int getVoiceInt(int voice);
  void setVoiceValue(int voice, double value);
  void setValue(double value) override;
  void addValue(double value);
  void addValueVoice(int voice, double value);
  void reset();

  double operator[](int voice) {
    if (voice == -1)
      return m_value;
    return m_values[voice];
  }
};

class FakeParameter : public Parameter {
public:
  FakeParameter(double value, InstanceID id);
};
} // namespace VeNo::Core