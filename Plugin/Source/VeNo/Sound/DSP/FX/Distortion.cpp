#include "Distortion.h"

#include "VUtils/Math.h"
#include "VeNo/Core/Instance.h"
namespace VeNo::Audio {
Distortion::Distortion(InstanceID id) : FX(id) {
  SetName("Distortion");
  m_Filter = CreateScope<Filter>(m_ID, "dist__filter", -1);
  auto &handler = Core::Instance::get(id)->handler;
  m_InputGain = handler->getParameter("dist__input_gain");
  m_OutputGain = handler->getParameter("dist__output_gain");
  m_Type = handler->getParameter("dist__type");

  m_Drive = handler->getModulateParameter("dist__drive");
  m_DynamicOne = handler->getModulateParameter("dist__dynamic1");
  m_Mix = handler->getModulateParameter("dist__mix");
}
void Distortion::update() {
  m_Mode = static_cast<DistortionMode>(m_Type->getInt() - 1);
  if (m_Mode == DistortionMode::OFF)
    return;
  m_Filter->setSampleRate(m_FXSampleRate);
  m_Filter->update();
}
void Distortion::process(Channel &channel) {
  if (m_Mode == DistortionMode::OFF)
    return;

  auto copyChannel = channel;
  copyChannel *= m_InputGain->getValue();
  m_Filter->process(copyChannel);
  double driveVal = VUtils::Math::dbToGain(m_Drive->getValue());
  copyChannel *= driveVal;
  switch (m_Mode) {
  case DistortionMode::SOFT: {
    constexpr auto factor = 2.f / juce::MathConstants<float>::pi;
    copyChannel.left = std::atan(copyChannel.left) * factor;
    copyChannel.right = std::atan(copyChannel.right) * factor;
    break;
  }
  case DistortionMode::HARD:
    copyChannel.left = VUtils::Math::clamp(copyChannel.left, -1, 1);
    copyChannel.right = VUtils::Math::clamp(copyChannel.right, -1, 1);
    break;
  case DistortionMode::HALF_RECTIFIER:
    copyChannel.left = copyChannel.left > 0 ? copyChannel.left : 0;
    copyChannel.right = copyChannel.right > 0 ? copyChannel.right : 0;
    break;
  case DistortionMode::RECTIFIER:
    copyChannel.left = abs(copyChannel.left);
    copyChannel.right = abs(copyChannel.right);
    break;
  case DistortionMode::SIN:
    copyChannel.left = std::sin(copyChannel.left);
    copyChannel.right = std::sin(copyChannel.right);
    break;
  case DistortionMode::TAN:
    copyChannel.left = std::tanh(copyChannel.left);
    copyChannel.right = std::tanh(copyChannel.right);
    break;
  case DistortionMode::FOLD_BACK: {
    double th = 1.0 + (m_DynamicOne->getValue() * 5.0);
    double th4 = th * 4;
    double th2 = th * 2;
    copyChannel.left = fabs(fabs(fmod(copyChannel.left - th, th4)) - th2) - th;
    copyChannel.right =
        fabs(fabs(fmod(copyChannel.right - th, th4)) - th2) - th;
    break;
  }
  case DistortionMode::END:
  case DistortionMode::OFF:
  default: break;
  }
  copyChannel *= (1 / (log(driveVal + 1) + 1));
  copyChannel *= m_OutputGain->getValue() * 0.5;
  double mix = m_Mix->getValue();
  channel.right = std::clamp(
      VUtils::Math::lerp(channel.right, copyChannel.right, mix), -1.0, 1.0);
  channel.left = std::clamp(
      VUtils::Math::lerp(channel.left, copyChannel.left, mix), -1.0, 1.0);
}
} // namespace VeNo::Audio