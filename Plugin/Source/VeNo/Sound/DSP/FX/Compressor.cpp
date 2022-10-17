#include "Compressor.h"

#include "VeNo/Core/Instance.h"

#include <VUtils/Math.h>
namespace VeNo::Audio {
Compressor::Compressor(InstanceID id) : FX(id) {
  auto *handler = Core::Instance::get(m_ID)->handler.get();
  SetName("Compressor");
  m_ThreshHoldParameter = handler->getParameter("compressor_threshold");
  m_RatioParameter = handler->getParameter("compressor_ratio");
  m_AttackParameter = handler->getParameter("compressor_attack");
  m_ReleaseParameter = handler->getParameter("compressor_release");
  m_IsActiveParameter = handler->getParameter("compressor_active");
}

void Compressor::update() {
  if (!m_IsActiveParameter->getBool())
    return;
  if (m_AttackParameter->getValue() != m_AttackMS) {
    m_AttackMS = m_AttackParameter->getValue();
    CalculateAttack();
  }
  if (m_ReleaseParameter->getValue() != m_ReleaseMS) {
    m_ReleaseMS = m_ReleaseParameter->getValue();
    CalculateRelease();
  }
  if (m_Threshold != m_ThreshHoldParameter->getValue()) {
    m_Threshold = m_ThreshHoldParameter->getValue();
    m_ThresholdGain = VUtils::Math::dbToGain(m_Threshold);
  }
}

void Compressor::process(Channel &channel) {
  if (!m_IsActiveParameter->getBool())
    return;

  auto leftAbs = std::abs(channel.left);
  auto rightAbs = std::abs(channel.right);
  auto sideInput = std::max(leftAbs, rightAbs);
  auto cte = (sideInput >= m_SampleRate ? m_AttackCT : m_ReleaseCT);
  auto env = sideInput + cte * (m_State - sideInput);
  m_State = env;

  auto cv = (env <= m_ThresholdGain
                 ? 1.f
                 : std::pow(env / m_ThresholdGain,
                            1.f / m_RatioParameter->getValue() - 1.f));
  channel *= cv;
}

void Compressor::setSampleRate(double sampleRate) {
  m_SampleRate = sampleRate;
  CalculateAttack();
  CalculateRelease();
}
void Compressor::CalculateAttack() {
  m_AttackCT = std::exp(-2.f * juce::MathConstants<float>::pi * 1000.f /
                        m_AttackMS / m_SampleRate);
}
void Compressor::CalculateRelease() {
  m_ReleaseCT = std::exp(-2.f * juce::MathConstants<float>::pi * 1000.f /
                         m_ReleaseCT / m_SampleRate);
}
} // namespace VeNo::Audio
