#include <VeNo/Core/Instance.h>
#include <VeNo/Events/EventCB.h>
#include <VeNo/Events/GUIEvents.h>
#include <VeNo/GUI/Components/WaveThumbnails.h>
#include <VeNo/GUI/Fonts/Icons.h>
#include <VeNo/Utils/WavePointUtils.h>

namespace VeNo::GUI {
WaveThumbnails::WaveThumbnails(const std::string &name,
                               const std::string &showName, size_t id)
    : BaseComponent(name, showName, id) {
  m_addButton = VComponent::create<Button>(m_id);
  m_addButton->setIcon(Icons::FontAwesome_Plus);
  m_addButton->pos.w = 20;
  m_addButton->pos.h = 20;
  m_addButton->setEventHandler(m_handler);
  m_addButton->setAction("wave-add-clicked");
  m_handler->addHandler("wave-add-clicked", this);
  m_handler->createOrGet<Events::EventCB>("wave-event")
      ->setCB([this](Events::Event *event) {
        auto waveEvent = event->as<WaveEditEvent>();
        if (waveEvent) {
          handleWaveThumb(waveEvent->thumbnail, waveEvent->type);
        }
      });
  m_components.push_back(m_addButton);
  addAndMakeVisible(m_addButton.get());
  setupFlex();
  updateSize();
}
void WaveThumbnails::resized() {
  Vector<Ref<ComponentGroup>> groups = {};
  m_flex.setItems(m_components, groups);
  m_flex.perform(
      juce::Rectangle<float>(0, 0, (float)getWidth(), (float)getHeight()));
}

void WaveThumbnails::updateSize() {
  Position size{};
  for (auto &component : m_components) {
    size.h += component->pos.h + 4;
  }
  size.w = getWidth();
  setSize(size.w, size.h);
}

void WaveThumbnails::setupFlex() {
  std::string direction = "column";
  std::string justify = "center";
  std::string alignContent = "space-between";
  m_flex.setDirection(direction);
  m_flex.setAlignItems(justify);
  m_flex.setJustifyContent(alignContent);
}
void WaveThumbnails::handle(Events::Event *event) {
  if (event->as<Events::ButtonClickedEvent>() != nullptr) {
    auto *instance = Core::Instance::get(m_id);
    auto *lib = instance->waveHolder.current();
    auto index = lib->createGroup()->index;
    createThumbnail(index, lib);
    m_currentWave = index;
    m_handler->triggerEvent("wave-change", new Events::ChangeEvent());
    updateSize();
  }
}
void WaveThumbnails::mouseUp(const juce::MouseEvent &event) {
  // ITS A DRAG NOT A CLICK ;)
  int dx = event.getDistanceFromDragStartY();
  if (dx > 5 || dx < -5 || event.mods.isPopupMenu())
    return;
  auto comp = dynamic_cast<WaveThumbnail *>(event.eventComponent);
  if (comp) {
    if (m_currentThumbnail) {
      m_currentThumbnail->isCurrent = false;
    }
    m_currentThumbnail = comp;
    m_currentThumbnail->isCurrent = true;
    m_currentWave = comp->thumbId;
    m_handler->triggerEvent("wave-change", new Events::ChangeEvent());
    repaint();
  }
}
void WaveThumbnails::createThumbnail(size_t index, Audio::WaveTableLib *lib) {
  if (m_currentThumbnail) {
    m_currentThumbnail->isCurrent = false;
  }
  auto thumbnail = VComponent::create<WaveThumbnail>(m_id);
  thumbnail->thumbId = index;
  thumbnail->lib = lib;
  thumbnail->pos.h = 40;
  thumbnail->pos.w = getWidth() - 10;
  thumbnail->setSize(thumbnail->pos.w, thumbnail->pos.h);
  thumbnail->addMouseListener(this, true);
  thumbnail->isCurrent = true;
  m_currentThumbnail = thumbnail.get();
  m_components.push_back(thumbnail);
  addAndMakeVisible(thumbnail.get());
}

void WaveThumbnails::createThumbnails() {
  auto *instance = Core::Instance::get(m_id);
  auto *lib = instance->waveHolder.current();
  for (size_t i = 0; i < lib->size(); ++i) {
    createThumbnail(i, lib);
  }
  updateSize();
}

void WaveThumbnails::regenerateCurrent() {
  m_components[m_currentWave + 1]->as<WaveThumbnail>()->generateWaveForm();
  repaint();
}

void WaveThumbnails::handleWaveThumb(WaveThumbnail *thumb,
                                     WaveEditType editType) {
  if (editType == WaveEditType::RESET) {
    thumb->lib->resetGroup(thumb->thumbId);
    thumb->generateWaveForm();
    thumb->repaint();
  } else if (editType == WaveEditType::DELETE) {
    thumb->lib->remove((long)thumb->thumbId);
    m_components.erase(m_components.begin() + long(thumb->thumbId + 1));
    removeChildComponent(thumb);
    if (m_currentWave > thumb->thumbId) {
      m_handler->triggerEvent("wave-change", new Events::ChangeEvent());
      m_currentWave--;
    }
    for (size_t i = 1; i < m_components.size(); ++i) {
      auto *newThumb = m_components[i]->as<WaveThumbnail>();
      newThumb->thumbId = i - 1;
    }
    updateSize();
  } else if (editType == WaveEditType::DUPLICATE) {
    size_t newIndex = Utils::WavePoint::duplicate(thumb->lib, thumb->thumbId);
    createThumbnail(newIndex, thumb->lib);
    m_currentWave = newIndex;
    m_handler->triggerEvent("wave-change", new Events::ChangeEvent());
    updateSize();
  } else if (editType == WaveEditType::INTERPOLATE) {
    Utils::WavePoint::interpolateWave(thumb->lib, m_currentWave, thumb->thumbId,
                                      10);
    m_components.erase(m_components.begin() + 1, m_components.end());
    createThumbnails();
    m_currentWave = 0;
    m_handler->triggerEvent("wave-change", new Events::ChangeEvent());
  }
}

void WaveThumbnail::paint(juce::Graphics &graphics) {
  auto *theme = Core::Config::get().theme().get();
  if (drawBackground) {
    graphics.setColour(theme->getColor(Theme::Colors::bgTwo));
    graphics.fillRect(0, 0, getWidth(), getHeight());
  }
  graphics.setColour(theme->getColor(isCurrent ? Theme::Colors::accentTwo
                                               : Theme::Colors::accent));
  juce::PathStrokeType strokeType{0.4f};
  graphics.strokePath(m_path, strokeType, juce::AffineTransform());
}

void WaveThumbnail::generateWaveForm() {
  auto &group = lib->getGroup(thumbId);
  size_t len = group->len;
  if (len == 0)
    return;

  auto &item = group->items[0];
  auto &data = item.items;
  juce::Path path{};
  float x = 0;
  float xInc = (float)getWidth() / (float)item.len;
  float lastY = -1;
  for (size_t i = 0; i < item.len; ++i) {
    float y = float((data[i] + 1) / 2) * (float)getHeight();
    if (lastY == -1)
      lastY = y;
    path.addLineSegment({x, lastY, x + xInc, y}, 0.05f);
    x += xInc;
    lastY = y;
  }
  m_path = path;
}

void WaveThumbnail::mouseDown(const juce::MouseEvent &event) {
  if (!event.mods.isPopupMenu() || !drawBackground)
    return;
  juce::PopupMenu m;
  if (!isCurrent) {
    m.addItem(1, "Delete");
    m.addItem(2, "Reset");
  }
  m.addItem(3, "Duplicate");
  m.addItem(4, "Interpolate");
  m.showMenuAsync({}, [this](int result) {
    switch (result) {
    case 1:
      m_handler->triggerEvent("wave-event",
                              new WaveEditEvent(WaveEditType::DELETE, this));
      break;
    case 2:
      m_handler->triggerEvent("wave-event",
                              new WaveEditEvent(WaveEditType::RESET, this));
      break;
    case 3:
      m_handler->triggerEvent("wave-event",
                              new WaveEditEvent(WaveEditType::DUPLICATE, this));
      break;
    case 4:
      m_handler->triggerEvent(
          "wave-event", new WaveEditEvent(WaveEditType::INTERPOLATE, this));
      break;
    default:
      // don't do anything because yeah we don't want to ;)
      break;
    }
  });
}
void WaveThumbnail::resized() { generateWaveForm(); }
WaveForm::WaveForm(const std::string &name, const std::string &showName,
                   size_t id)
    : BaseComponent(name, showName, id) {
  m_thumbnail = CreateRef<WaveThumbnail>(m_name, m_showName, m_id);
  addAndMakeVisible(*m_thumbnail);
}

void WaveForm::init() {
  if (m_isOscillator) {
    auto string = "osc" + std::to_string(m_waveId + 1) + "__wave_position";
    m_handler->addHandler(string, this);
  }
  std::string editor_closed = "wave-editor-closed_" + std::to_string(m_waveId);
  m_handler->addHandler(editor_closed, this);
  auto *instance = Core::Instance::get(m_id);
  m_thumbnail->drawBackground = false;
  m_thumbnail->lib = instance->waveHolder.generators[m_waveId].get();
  m_thumbnail->thumbId = 0; // this will update correctly based on changes ;)
  m_thumbnail->setSize(getWidth(), getHeight());
  m_isInit = true;
}

void WaveForm::resized() {
  if (m_thumbnail && m_isInit) {
    m_thumbnail->setSize(getWidth(), getHeight());
  }
}

void WaveForm::handle(Events::Event *) {
  if (m_isOscillator) {
    auto string = "osc" + std::to_string(m_waveId + 1) + "__wave_position";
    auto val =
        Core::Instance::get(m_id)->treeState->getParameter(string)->getValue();
    auto index = (size_t)std::floor(val * ((double)m_thumbnail->lib->size() - 1));
    if (m_thumbnail->thumbId != index || m_thumbnail->lib->isOutdated()) {
      m_thumbnail->thumbId = index;
      m_thumbnail->generateWaveForm();
      repaint();
    }
  } else {
    m_thumbnail->generateWaveForm();
    repaint();
  }
}
} // namespace VeNo::GUI