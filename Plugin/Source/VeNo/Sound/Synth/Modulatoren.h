#pragma once
#include "VeNo/Utils/Random.h"

#include <VeNo/Events/Event.h>
#include <VeNo/Sound/Generation/EnvelopeStructs.h>
#include <VeNo/Sound/Generation/LFO.h>
#include <VeNo/TypeDefs.h>

namespace VeNo::Audio {
class Matrix;
class ModulatorHandle;
class Modulator {
public:
  explicit Modulator(InstanceID id) : m_id(id) {}
  virtual ~Modulator() = default;
  virtual void update() = 0;
  virtual double value(int index) = 0;
  virtual bool isVoiceModulator() { return false; }
  virtual VString &name() = 0;

protected:
  InstanceID m_id{0};
  double m_SampleRate = 44100.0;
  friend ModulatorHandle;
};
// Wrapping Env2-4
class EnvModulator : public Modulator, Events::Handler {
public:
  explicit EnvModulator(InstanceID id) : Modulator(id) {}
  void init(int envelope = 2);
  void update() override;
  double value(int index) override;
  void handle(Events::Event *event) override;
  void setSampleRate(double sampleRate);
  EnvelopeData &data() { return m_data; }
  EnvelopeVoiceData &voiceData(int voice) { return m_voiceData[voice]; }
  bool isVoiceModulator() override;

  VString &name() override { return m_name; }

protected:
  VString m_name{};
  EnvelopeData m_data{};
  EnvelopeVoiceData m_voiceData[MAX_VOICES + 1];
  int m_envelope{2};
};
// Random Data ;)
class RandomModulator : public Modulator {
public:
  explicit RandomModulator(InstanceID id) : Modulator(id) {}
  void init(int index);
  void update() override;
  double value(int index) override;
  VString &name() override { return m_name; }

protected:
  VString m_name;
  Core::ModulateParameter *m_ChangeRate{nullptr};
  Core::Parameter *m_Active{nullptr};
  Core::Parameter *m_Mode{nullptr};
  double m_Value{0};
  int m_Samples{0};
  Utils::Random m_Random{};
  double m_Values[5]{0, 0, 0, 0, 0};
};

class LFOModulator : public Modulator {
public:
  explicit LFOModulator(InstanceID id) : Modulator(id) {}
  void init(int number);
  void update() override;
  double value(int index) override;

  VString &name() override { return m_name; }

protected:
  VString m_name{"LFO"};
  LFOData m_Data{};
  WaveLib *m_WaveLib{nullptr};
};

class ModKnobsModulator : public Modulator {
public:
  explicit ModKnobsModulator(InstanceID id) : Modulator(id) {}
  void init(int index);
  void init(const VString &name, int index);
  void update() override;
  double value(int index) override;

  VString &name() override { return m_Parameter->getShowName(); }

protected:
  Core::Parameter *m_Parameter{nullptr};
  double m_Value{0.0};
};

class ModulatorHandle {
public:
  explicit ModulatorHandle(InstanceID id) : m_id(id) {}
  void init(Matrix *matrix);
  void setSampleRate(double sampleRate);
  void triggerNoteOn(int voice);
  void triggerNoteOff(int voice);

protected:
  Vector<Ref<LFOModulator>> m_LFOs;
  Vector<Ref<RandomModulator>> m_randomGenerators;
  Vector<Ref<EnvModulator>> m_envelopes;
  Vector<Ref<ModKnobsModulator>> m_knobsModulator;
  InstanceID m_id{};
};
} // namespace VeNo::Audio