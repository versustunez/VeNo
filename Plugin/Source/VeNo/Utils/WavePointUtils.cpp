#include "VeNo/Core/Instance.h"

#include <VeNo/Sound/Tables/WaveTable.h>
#include <VeNo/Utils/WavePointUtils.h>
#include <algorithm>
#include <cmath>

namespace VeNo::Utils {
#define SNAP_SIZE 0.0625f

bool sort(Audio::WavePoint &a, Audio::WavePoint &b) {
  return a.data.x < b.data.x;
}

WavePoints WavePoint::findNearest(Audio::WaveUIPoints &points, float x,
                                  float y) {
  constexpr float DOT_SIZE = 0.015;
  for (auto &point : points.points) {
    float distX = std::abs(point.data.x - x);
    float distY = std::abs(point.data.y - y);
    if (distX < DOT_SIZE && distY < DOT_SIZE) {
      return {&point};
    }
    // curve points also have the move version
    if (point.bezier) {
      distX = std::abs(point.curved.x - x);
      distY = std::abs(point.curved.y - y);
      if (distX < DOT_SIZE && distY < DOT_SIZE) {
        return {&point, true};
      }
    }
  }
  return {};
}
bool WavePoint::updatePoint(Audio::WaveUIPoints &, float x, float y,
                            Utils::WavePoints &currentPoint, bool shitDown,
                            bool ctrlDown) {

  auto *point = currentPoint.point;
  if (!point)
    return false;
  if (currentPoint.isCurvedPoint) {
    point->curved.x = x;
    point->curved.y = y;
    point->curved.value = ((y * 2) - 1) * -1;
    return true;
  }
  if (shitDown)
    point->data.y = std::roundf(y / SNAP_SIZE) * SNAP_SIZE;
  else
    point->data.y = y;
  point->data.value = ((point->data.y * 2) - 1) * -1;

  if (!point->isEdge) {
    // We have to look if we can move
    float nx = x;
    if (ctrlDown)
      nx = std::roundf(x / SNAP_SIZE) * SNAP_SIZE;

    float leftX = 0;
    float rightX = 1;
    if (point->previous)
      leftX = point->previous->data.x;
    if (point->next)
      rightX = point->next->data.x;

    // offset them! :P
    leftX += 0.01;
    rightX -= 0.01;
    if (nx > leftX && nx < rightX)
      point->data.x = nx;
  }
  return true;
}
bool WavePoint::removeCurrentPoint(Audio::WaveUIPoints &points,
                                   WavePoints &currentPoint) {
  if (currentPoint.point && !currentPoint.isCurvedPoint) {
    for (size_t i = 0; i < points.points.size(); ++i) {
      if (&points.points[i] == currentPoint.point) {
        if (!currentPoint.point->isEdge) {
          points.points.erase(points.points.begin() + (long)i);
          finish(points);
          return true;
        }
        break;
      }
    }
  }
  return false;
}
void WavePoint::finish(Audio::WaveUIPoints &points) {
  std::sort(points.points.begin(), points.points.end(), sort);
  points.updateNeighbours();
  points.updateCurved();
}
void WavePoint::triggerUpdate(Audio::WaveTableLib *waveLib, size_t index) {
  auto group = CreateRef<Audio::WaveTableGroup>();
  group->uiPoints = waveLib->getGroup(index)->uiPoints;
  group->uiPoints.updateNeighbours();
  Audio::WaveTableCreator::generate(group->uiPoints.points, group.get());
  waveLib->replaceGroup(index, group);
  Core::Instance::get(waveLib->id())->eventHandler.triggerEvent("wave-update", new Events::ChangeEvent());
}
} // namespace VeNo::Utils