#pragma once

#include "BaseComponent.h"
#include "Label.h"
#include "VeNo/Sound/DSP/FXChain.h"

namespace VeNo::GUI {
class FXListSort;
class FXListItem : public Label, public juce::DragAndDropTarget {
public:
  FXListItem(const std::string &name, const std::string &showName,
             InstanceID id);
  void paint(juce::Graphics &g) override;
  void mouseDrag(const juce::MouseEvent &event) override;
  bool
  isInterestedInDragSource(const SourceDetails &dragSourceDetails) override;
  void itemDropped(const SourceDetails &dragSourceDetails) override;

public:
  FXListSort *ListSortContainer{nullptr};
  size_t Index{0};
  size_t OriginalIndex{0};
};
class FXListSort : public BaseComponent,
                   public Events::Handler,
                   public juce::DragAndDropContainer,
                   public juce::DragAndDropTarget {
public:
  FXListSort(const std::string &name, const std::string &showName,
             InstanceID id);
  ~FXListSort() override;
  void Init();
  void handle(Events::Event *event) override;
  void resized() override;

  void HandleInsertAfter(Component *toInsert, FXListItem *currentContainer,
                         bool insertAfter);
  bool
  isInterestedInDragSource(const SourceDetails &dragSourceDetails) override;
  void itemDropped(const SourceDetails &dragSourceDetails) override;

protected:
  void sortByList();
  void sortByChildren();
  void setElementPositions();
  void UpdateIndex();

protected:
  Audio::FXChain *m_FXChain{nullptr};
  Dequeue<Ref<FXListItem>> m_Components{};
};
} // namespace VeNo::GUI
