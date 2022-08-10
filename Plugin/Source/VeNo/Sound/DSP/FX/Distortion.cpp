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
  m_DynamicTwo = handler->getModulateParameter("dist__dynamic2");
  m_Mix = handler->getModulateParameter("dist__mix");
}
void Distortion::update() {
  m_Mode = m_Type->getInt() - 1;
  if (m_Mode == DistortionMode::OFF)
    return;
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
  case DistortionMode::SOFT:
    copyChannel.left = std::atan(copyChannel.left);
    copyChannel.right = std::atan(copyChannel.right);
    break;
  case DistortionMode::HARD:
    copyChannel.left = VUtils::Math::clamp(copyChannel.left, -1, 1);
    copyChannel.right = VUtils::Math::clamp(copyChannel.right, -1, 1);
    break;
  case DistortionMode::RECTIFIER: {
    copyChannel.left = copyChannel.left > 0 ? copyChannel.left : 0;
    copyChannel.right = copyChannel.right > 0 ? copyChannel.right : 0;
    break;
  } case DistortionMode::EXPERIMENTAL:
  default:
    break;
  }
  copyChannel *= (1 / (log(driveVal + 1) + 1));
  copyChannel *= m_OutputGain->getValue();
  double mix = m_Mix->getValue();
  channel.right = VUtils::Math::lerp(channel.right, copyChannel.right, mix);
  channel.left = VUtils::Math::lerp(channel.left, copyChannel.left, mix);
}
} // namespace VeNo::Audio