#pragma once
namespace VeNo {

class FilterImpl {
public:
  enum class Type : size_t {
    NO,
    LowShelf,
    HighShelf,
    Peak,
    BandPass,
    HighPass,
    LowPass,
    END
  };

  bool SetFilterType(Type type);
  bool SetFilterTypeByNumber(int number);
  void SetSampleRate(double sampleRate);
  void Reset();
  double ApplyLeft(double in);
  double ApplyRight(double in);
  bool IsBypassed() const { return m_Type == Type::NO; }

  void CalculateCoefficients(double gain, double frequency, double q = 0.0);

  double GetMagnitudeForFrequency(double frequency, double sampleRate) const;

protected:
  Type m_Type{Type::NO};
  double m_SampleRate{44100.0f};

  double m_B0{0}, m_B1{0}, m_B2{0};
  double m_A0{0}, m_A1{0}, m_A2{0};

  double m_xL1{0}, m_xL2{0};
  double m_yL1{0}, m_yL2{0};

  double m_xR1{0}, m_xR2{0};
  double m_yR1{0}, m_yR2{0};
};

} // namespace VeNo
