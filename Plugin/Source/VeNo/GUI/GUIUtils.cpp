#include <VUtils/StringUtils.h>
#include <VeNo/Core/Config.h>
#include <VeNo/GUI/GUIUtils.h>

typedef VeNo::Core::Config Config;

namespace VeNo::GUI {
double Utils::setFontSize(double size, juce::Graphics &g) {
  double scale = Config::get().m_scale;
  auto s = (double)(size * scale);
  g.setFont((float)s);
  return s;
}
int Utils::getScaledSize(int size) {
  return static_cast<int>(size * Config::get().m_scale);
}
float Utils::getScaledSize(float size) {
  return static_cast<float>(size * Config::get().m_scale);
}
juce::Path Utils::drawRectangleWithRadius(
    int x, int y, int w, int h, float radius, juce::Graphics &g) {
  juce::Path p;
  p.startNewSubPath(x, y);
  p.lineTo(x + w - radius, y);
  p.addArc(x + w - radius, y, radius, radius, 0.0f, M_PI);
  p.lineTo(x + w, y + h - radius);
  p.addArc(x + w - radius, y + h - radius, radius, radius, M_PI_2, M_PI);
  p.lineTo(x, y + h);
  p.closeSubPath();
  g.fillPath(p);
  return p;
}
Core::Instance *Utils::getInstanceOrNull(const juce::Component &component) {
  std::string compid = component.getComponentID().toStdString();
  long pid = VUtils::StringUtils::toNumber(compid, -1);
  if (pid == -1)
    return nullptr;
  return Core::Instance::get((size_t)pid);
}
} // namespace VeNo::GUI