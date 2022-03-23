#include <VeNo/Core/Config.h>
#include <VeNo/Core/Instance.h>
#include <VeNo/Events/EventCB.h>
#include <VeNo/GUI/Components/ScrollComponent.h>
#include <VeNo/GUI/Components/WaveEditor.h>
#include <VeNo/Utils/WavePointUtils.h>
#include <VeNo/Window/WaveEditor.h>
#include <utility>

namespace VeNo::GUI {
WaveEditor::WaveEditor(std::string name, const std::string &showName, size_t id)
    : BaseComponent(std::move(name), showName, id) {
  auto *instance = Core::Instance::get(id);
  m_lib =
      instance->waveHolder.generators[instance->state.currentEditingOscillator]
          .get();
  m_handler->addHandler("wave-change", this);
  m_handler->createOrGet<Events::EventCB>("wave-update")
      ->setCB([this](Events::Event *) { m_thumbnails->regenerateCurrent(); });
  m_currentThumbnail = VComponent::create<WaveThumbnail>(m_id);
  m_currentThumbnail->drawBackground = false;
  m_currentThumbnail->lib = m_lib;
  m_currentThumbnail->addMouseListener(this, true);
  addAndMakeVisible(m_currentThumbnail.get());
}

void WaveEditor::afterParsing(Interpreter *interpreter) {
  auto thumbnailComponent = interpreter->find("WaveThumbnails");
  auto xComponent = interpreter->find("xPoint");
  auto yComponent = interpreter->find("yPoint");
  if (thumbnailComponent == nullptr || xComponent == nullptr ||
      yComponent == nullptr) {
    ERR("Something went wrong!");
    return;
  }
  auto scrollComp = thumbnailComponent->as<ScrollComponent>()->component();
  m_thumbnails = scrollComp->as<WaveThumbnails>();
  m_xPointComponent = (Label *)xComponent;
  m_yPointComponent = (Label *)yComponent;
}

void WaveEditor::paint(juce::Graphics &g) {
  if (m_color.hasColor) {
    auto *theme = Core::Config::get().theme().get();
    if (m_color.isPreColor)
      g.setColour(theme->getColor(m_color.preColor));
    else
      g.setColour(juce::Colour(m_color.colors[0], m_color.colors[1],
                               m_color.colors[2], m_color.colors[3]));
    g.fillAll();
  }
}

void WaveEditor::paintOverChildren(juce::Graphics &g) {
  drawDots(g);
}

void WaveEditor::resized() {
  m_currentThumbnail->setSize(getWidth(), getHeight());
  m_currentThumbnail->generateWaveForm();
}

void WaveEditor::setColorComponent(const GUIColorComponent &colorComponent) {
  m_color = colorComponent;
}

void WaveEditor::handle(Events::Event *) {
  if (m_thumbnails) {
    m_currentPoint.point = nullptr;
    m_currentPoint.isCurvedPoint = false;
    m_currentWave = m_thumbnails->currentWave();
    m_currentThumbnail->thumbId = m_currentWave;
    m_currentThumbnail->generateWaveForm();
    repaint();
  }
}

void WaveEditor::drawDots(juce::Graphics &graphics) {
  // draw from current lib dots ;)
  if (m_lib->size() == 0)
    return;
  constexpr float GUI_DOT_SIZE = 0.041f;
  auto &group = m_lib->getGroup(m_currentWave);
  auto &points = group->uiPoints.points;
  auto height = (float)getHeight();
  auto width = (float)getWidth();
  float dotSize = height * GUI_DOT_SIZE;
  float halfDot = dotSize / 2;
  auto *theme = Core::Config::get().theme().get();
  graphics.setColour(theme->getDirectColor(Theme::Colors::accent));
  for (auto &point : points) {
    if (m_currentPoint.point == &point)
      continue;
    graphics.fillEllipse(point.data.x * width - halfDot,
                         point.data.y * height - halfDot, dotSize, dotSize);
    if (point.bezier && point.next)
      graphics.drawEllipse(point.curved.x * width - halfDot,
                           point.curved.y * height - halfDot, dotSize, dotSize,
                           1.0f);
  }

  if (m_currentPoint.point) {
    auto *point = m_currentPoint.point;
    graphics.setColour(theme->getDirectColor(Theme::Colors::accentTwo));
    if (point->bezier && point->next)
      graphics.drawEllipse(point->curved.x * width - halfDot,
                           point->curved.y * height - halfDot, dotSize, dotSize,
                           1.0f);
    graphics.fillEllipse(point->data.x * width - halfDot,
                         point->data.y * height - halfDot, dotSize, dotSize);
  }
}

void WaveEditor::mouseDrag(const juce::MouseEvent &event) {
  if (!m_currentPoint.point) {
    return;
  }

  float yPos = std::clamp((float)event.y / (float)getHeight(), 0.0f, 1.0f);
  float xPos = std::clamp((float)event.x / (float)getWidth(), 0.0f, 1.0f);
  auto &uiPoints = m_lib->getGroup(m_currentWave)->uiPoints;
  if (Utils::WavePoint::updatePoint(uiPoints, xPos, yPos, m_currentPoint,
                                    event.mods.isShiftDown(),
                                    event.mods.isCtrlDown())) {
    if (m_xPointComponent && m_yPointComponent) {
      m_xPointComponent->setText(std::to_string(m_currentPoint.point->data.x));
      m_yPointComponent->setText(std::to_string(m_currentPoint.point->data.y));
    }
    m_changed = true;
    repaint();
  }
}

void WaveEditor::mouseDown(const juce::MouseEvent &event) {
  float yPos = std::clamp((float)event.y / (float)getHeight(), 0.0f, 1.0f);
  float xPos = std::clamp((float)event.x / (float)getWidth(), 0.0f, 1.0f);
  auto &uiPoints = m_lib->getGroup(m_currentWave)->uiPoints;
  auto point = Utils::WavePoint::findNearest(uiPoints, xPos, yPos);
  if (point.point != m_currentPoint.point) {
    m_currentPoint = point;
    repaint();
  }
}
void WaveEditor::mouseUp(const juce::MouseEvent &event) { triggerEnd(event); }

void WaveEditor::mouseDoubleClick(const juce::MouseEvent &event) {
  float yPos = std::clamp((float)event.y / (float)getHeight(), 0.0f, 1.0f);
  float xPos = std::clamp((float)event.x / (float)getWidth(), 0.0f, 1.0f);
  auto &uiPoints = m_lib->getGroup(m_currentWave)->uiPoints;
  m_changed = true;
  if (Utils::WavePoint::removeCurrentPoint(uiPoints, m_currentPoint)) {
    triggerEnd(event);
    return;
  };
  if (m_currentPoint.isCurvedPoint) {
    auto point = m_currentPoint.point;
    point->curved.x = (point->data.x + point->next->data.x) / 2;
    point->curved.y = (point->data.y + point->next->data.y) / 2;
    point->curved.value = (point->data.value + point->next->data.value) / 2;
    triggerEnd(event);
    return;
  }
  if (event.mods.isShiftDown()) {
    uiPoints.addCurvedPoint(xPos, yPos, (yPos * 2 - 1) * -1, {}, false);
  } else {
    uiPoints.addPoint(xPos, yPos, (yPos * 2 - 1) * -1, false);
  }
  Utils::WavePoint::finish(uiPoints);
  triggerEnd(event);
}

void WaveEditor::triggerEnd(const juce::MouseEvent &event) {
  if (!m_changed)
    return;
  m_changed = false;
  Utils::WavePoint::triggerUpdate(m_lib, m_currentWave);
  float yPos = std::clamp((float)event.y / (float)getHeight(), 0.0f, 1.0f);
  float xPos = std::clamp((float)event.x / (float)getWidth(), 0.0f, 1.0f);
  m_currentPoint = Utils::WavePoint::findNearest(
      m_lib->getGroup(m_currentWave)->uiPoints, xPos, yPos);
  m_currentThumbnail->generateWaveForm();
  repaint();
}

} // namespace VeNo::GUI