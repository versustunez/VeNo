#include <VeNo/Core/Instance.h>
#include <VeNo/Events/EventCB.h>
#include <VeNo/Events/GUIEvents.h>
#include <VeNo/GUI/Components/WaveThumbnails.h>
#include <VeNo/GUI/Fonts/Icons.h>

namespace VeNo::GUI {
class WaveEditEvent : public Events::NoHandleEvent {
public:
  WaveEditEvent(bool _isEdit, GUI::WaveThumbnail *_thumbnail)
      : isEdit(_isEdit),
        thumbnail(_thumbnail) {}
  bool isEdit;
  GUI::WaveThumbnail *thumbnail;
};
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
          handleWaveThumb(waveEvent->thumbnail, waveEvent->isEdit);
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
    auto *lib = instance->waveHolder
                    .generators[instance->state.currentEditingOscillator]
                    .get();
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
  thumbnail->generateWaveForm();
  thumbnail->isCurrent = true;
  m_currentThumbnail = thumbnail.get();
  m_components.push_back(thumbnail);
  addAndMakeVisible(thumbnail.get());
}

void WaveThumbnails::createThumbnails() {
  auto *instance = Core::Instance::get(m_id);
  auto *lib =
      instance->waveHolder.generators[instance->state.currentEditingOscillator]
          .get();
  for (size_t i = 0; i < lib->size(); ++i) {
    createThumbnail(i, lib);
  }
  updateSize();
}

void WaveThumbnails::regenerateCurrent() {
  m_components[m_currentWave + 1]->as<WaveThumbnail>()->generateWaveForm();
  repaint();
}

void WaveThumbnails::handleWaveThumb(WaveThumbnail *thumb, bool isEdit) {
  if (isEdit) {
    thumb->lib->resetGroup(thumb->thumbId);
    thumb->generateWaveForm();
    thumb->repaint();
  } else {
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
  }
}

void WaveThumbnail::paint(juce::Graphics &graphics) {
  auto *theme = Core::Config::get().theme().get();
  if (drawBackground) {
    graphics.setColour(theme->getDirectColor(Theme::Colors::bgTwo));
    graphics.fillRect(0, 0, getWidth(), getHeight());
  }
  graphics.setColour(theme->getDirectColor(isCurrent ? Theme::Colors::accentTwo
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
  if (event.mods.isPopupMenu() && !isCurrent && drawBackground) {
    juce::PopupMenu m;
    m.addItem(1, "Delete");
    m.addItem(2, "Reset");
    m.showMenuAsync({}, [this](int result) {
      switch (result) {
      case 1:
        m_handler->triggerEvent("wave-event", new WaveEditEvent(false, this));
        break;
      case 2:
        m_handler->triggerEvent("wave-event", new WaveEditEvent(true, this));
        break;
      default:
        // don't do anything because yeah we don't want to ;)
        break;
      }
    });
  }
}
} // namespace VeNo::GUI