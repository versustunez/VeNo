#pragma once
#include <VeNo/Events/Event.h>
#include <VeNo/Sound/Generation/EnvelopeStructs.h>
#include <VeNo/Sound/Generation/LFO.h>
#include <VeNo/TypeDefs.h>

namespace VeNo::Audio {
class Matrix;
class Modulator {
public:
  explicit Modulator(InstanceID id) : m_id(id){};
  virtual ~Modulator() = default;
  virtual void update() = 0;
  virtual double value(int index) = 0;
  virtual VString &name() = 0;

protected:
  InstanceID m_id{0};
};
// Wrapping Env2-4
class EnvModulator : public Modulator, Events::Handler {
public:
  explicit EnvModulator(InstanceID id) : Modulator(id) { init(); };
  void init(int envelope = 2);
  void update() override;
  double value(int index) override;
  void handle(Events::Event *event) override;
  void setSampleRate(double sampleRate);
  EnvelopeData &data() { return m_data; }
  EnvelopeVoiceData &voiceData(int voice) { return m_voiceData[voice]; }

  VString &name() override { return m_name; };

protected:
  VString m_name{};
  EnvelopeData m_data{};
  EnvelopeVoiceData m_voiceData[MAX_VOICES];
  int m_envelope{2};
};
// Random Data ;)
class RandomModulator : public Modulator {
public:
  explicit RandomModulator(InstanceID id) : Modulator(id) { init(); };
  void init();
  void update() override;
  double value(int index) override;
  VString &name() override { return m_name; }

protected:
  VString m_name{"LFO"};
};
class LFOModulator : public Modulator {
public:
  explicit LFOModulator(InstanceID id) : Modulator(id) { init(); };
  void init();
  void update() override;
  double value(int index) override;

  VString &name() override { return m_name; }

protected:
  VString m_name{"LFO"};
  LFOData m_data;
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
  InstanceID m_id{};
};
} // namespace VeNo::Audio