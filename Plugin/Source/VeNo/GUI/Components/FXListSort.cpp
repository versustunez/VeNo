#include "FXListSort.h"

#include "VeNo/Core/Instance.h"
#define CONTAINER_HEIGHT 60
#define CONTAINER_SPACER 5

namespace VeNo::GUI {

FXListItem::FXListItem(const std::string &name, const std::string &showName,
                       InstanceID id)
    : Label(name, showName, id) {
  addMouseListener(this, true);
}
void FXListItem::paint(juce::Graphics &g) {
  auto *theme = Core::Config::get().theme().get();
  g.fillAll((Index & 1) == 0 ? theme->getColor(Theme::Colors::bg)
                             : theme->getColor(Theme::Colors::bgTwo));
  g.setColour(theme->getColor(Theme::Colors::accent));
  g.fillRect(0, getHeight() - 1, getWidth(), 1);
}
void FXListItem::mouseDrag(const juce::MouseEvent &) {
  if (ListSortContainer && !ListSortContainer->isDragAndDropActive()) {
    ListSortContainer->startDragging("Sorting :>", this);
  }
}
bool FXListItem::isInterestedInDragSource(
    const juce::DragAndDropTarget::SourceDetails &) {
  return true;
}
void FXListItem::itemDropped(
    const juce::DragAndDropTarget::SourceDetails &details) {
  ListSortContainer->HandleInsertAfter(details.sourceComponent.get(), this,
                                       details.localPosition.y >
                                           getHeight() / 2);
}

FXListSort::FXListSort(const std::string &name, const std::string &showName,
                       InstanceID id)
    : BaseComponent(name, showName, id) {
  m_handler->addHandler("fx-serialized", this);
}

FXListSort::~FXListSort() { m_handler->removeHandler("fx-serialized"); }

void FXListSort::Init() {
  auto *instanceState = Core::Instance::getState(m_id);
  m_FXChain = instanceState->FXChain.get();
  // Build Child list ;)
  auto &fxData = m_FXChain->GetFXData();
  size_t index = 0;
  for (auto &i : fxData) {
    auto item = VComponent::create<FXListItem>(m_id);
    item->setText(i->GetName());
    item->ListSortContainer = this;
    item->Index = m_Components.size();
    item->OriginalIndex = index++;
    m_Components.push_back(item);
    addAndMakeVisible(*item);
  }
  sortByList();
}

void FXListSort::handle(Events::Event *) {
  // Sort Childs :>
  sortByList();
  setElementPositions();
}

void FXListSort::sortByList() {
  auto &sorted = m_FXChain->GetSorted(); // Sorted Index Data
  size_t index = 0;
  for (auto idx : sorted) {
    for (auto &component : m_Components) {
      if (component->OriginalIndex == idx) {
        component->Index = index;
        break;
      }
    }
    index++;
  }
  std::sort(m_Components.begin(), m_Components.end(),
            [&](auto &A, auto &B) { return A->Index <= B->Index; });
}

void FXListSort::sortByChildren() {
  std::sort(m_Components.begin(), m_Components.end(),
            [&](auto &A, auto &B) { return A->Index <= B->Index; });
  Vector<size_t> sortList;
  for (auto &component : m_Components) {
    sortList.push_back(component->OriginalIndex);
  }
  m_FXChain->SetSortedList(sortList);
}
void FXListSort::resized() { setElementPositions(); }
void FXListSort::setElementPositions() {
  int y = 20;
  for (auto &m_Component : m_Components) {
    m_Component->setBounds(0, y, getWidth(), CONTAINER_HEIGHT);
    y += CONTAINER_HEIGHT + CONTAINER_SPACER;
  }
}

void FXListSort::HandleInsertAfter(juce::Component *toInsert,
                                   FXListItem *currentContainer,
                                   bool insertAfter) {
  // Insert after that container :D
  auto item = reinterpret_cast<FXListItem *>(toInsert);
  if (!item)
    return ERR("Item is not of Type: FXListItem");
  if (item == currentContainer) {
    return;
  }
  auto element = m_Components[item->Index];
  if ((currentContainer->Index == item->Index + 1 && !insertAfter) ||
      (insertAfter && currentContainer->Index == item->Index - 1)) {
    return;
  }
  m_Components.erase(m_Components.begin() + (int)item->Index);
  size_t index = 0;
  for (size_t i = 0; i < m_Components.size(); ++i) {
    if (m_Components[i].get() == currentContainer) {
      index = i;
      break;
    }
  }
  if (insertAfter) {
    m_Components.insert(m_Components.begin() + (int)index + 1, element);
  } else {
    if (index == 0) {
      m_Components.push_front(element);
    } else {
      m_Components.insert(m_Components.begin() + (int)index - 1, element);
    }
  }
  UpdateIndex();
  sortByChildren();
  setElementPositions();
}
bool FXListSort::isInterestedInDragSource(
    const juce::DragAndDropTarget::SourceDetails &) {
  return true;
}
void FXListSort::itemDropped(
    const juce::DragAndDropTarget::SourceDetails &details) {
  auto item = reinterpret_cast<FXListItem *>(details.sourceComponent.get());
  if (!item)
    return ERR("Item is not of Type: FXListItem");

  auto element = m_Components[item->Index];
  m_Components.erase(m_Components.begin() + (int)item->Index);
  if (details.localPosition.y >
      int(m_Components.size() * CONTAINER_HEIGHT + CONTAINER_SPACER) + 20) {
    m_Components.push_back(element);
  } else if (details.localPosition.getY() < 20) {
    m_Components.push_front(element);
  }
  UpdateIndex();
  sortByChildren();
  setElementPositions();
}
void FXListSort::UpdateIndex() {
  for (size_t i = 0; i < m_Components.size(); ++i) {
    m_Components[i]->Index = i;
  }
}
} // namespace VeNo::GUI