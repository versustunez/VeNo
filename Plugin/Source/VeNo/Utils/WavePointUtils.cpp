#include <VUtils/Math.h>
#include <VeNo/Core/Instance.h>
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
  Core::Instance::get(waveLib->id())
      ->eventHandler.triggerEvent("wave-update", new Events::ChangeEvent());
}
size_t WavePoint::duplicate(Audio::WaveTableLib *waveLib, size_t index) {
  auto group = CreateRef<Audio::WaveTableGroup>();
  group->uiPoints = waveLib->getGroup(index)->uiPoints;
  group->uiPoints.updateNeighbours();
  Audio::WaveTableCreator::generate(group->uiPoints.points, group.get());
  waveLib->addGroup(group);
  return group->index;
}

void WavePoint::interpolateWave(Audio::WaveTableLib *waveLib, size_t first,
                                size_t end, int times) {
  if (end < first) {
    size_t temp = first;
    first = end;
    end = temp;
  }
  auto firstG = waveLib->getGroup(first);
  auto endG = waveLib->getGroup(end);

  // not the same amount of points? yeah goodbye ^^
  // @TODO: Add support for not same point count
  if (firstG->uiPoints.points.size() != endG->uiPoints.points.size())
    return;

  auto &firstPoints = firstG->uiPoints.points;
  auto &otherPoints = endG->uiPoints.points;
  for (int i = 0; i < times; ++i) {
    // need to interpolate between them :O
    auto group = CreateRef<Audio::WaveTableGroup>();
    // if both are curved the interpolation is easy... if not we have a problem
    // :(
    double factor = (double)i / (double)times;
    for (size_t pointIdx = 0; pointIdx < firstPoints.size(); ++pointIdx) {
      auto &pointOne = firstPoints[pointIdx];
      auto &pointTwo = otherPoints[pointIdx];
      // clang-format off
      auto x = (float)VUtils::Math::lerp(pointOne.data.x, pointTwo.data.x,factor);
      auto y = (float)VUtils::Math::lerp(pointOne.data.y, pointTwo.data.y,factor);
      auto val = (float)VUtils::Math::lerp(pointOne.data.value, pointTwo.data.value,factor);
      if (pointOne.bezier && pointTwo.bezier) {
        // easy mode
        auto cx = (float)VUtils::Math::lerp(pointOne.curved.x, pointTwo.curved.x,factor);
        auto cy = (float)VUtils::Math::lerp(pointOne.curved.y, pointTwo.curved.y,factor);
        auto cval = (float)VUtils::Math::lerp(pointOne.curved.value, pointTwo.curved.value,factor);
        group->uiPoints.addCurvedPoint(x,y, val, {cx, cy, cval}, pointOne.isEdge);
      } else if (!pointOne.bezier && !pointTwo.bezier) {
        // easy too
        group->uiPoints.addPoint(x,y, val, pointOne.isEdge);
      } else if (pointOne.bezier) {
        // harder
        auto cx = (float)VUtils::Math::lerp(pointOne.curved.x, pointTwo.data.x,factor);
        auto cy = (float)VUtils::Math::lerp(pointOne.curved.y, pointTwo.data.y,factor);
        auto cval = (float)VUtils::Math::lerp(pointOne.curved.value, pointTwo.data.value,factor);
        group->uiPoints.addCurvedPoint(x,y, val, {cx, cy, cval}, pointOne.isEdge);
      } else if (pointTwo.bezier) {
        auto cx = (float)VUtils::Math::lerp(pointOne.data.x, pointTwo.curved.x,factor);
        auto cy = (float)VUtils::Math::lerp(pointOne.data.y, pointTwo.curved.y,factor);
        auto cval = (float)VUtils::Math::lerp(pointOne.data.value, pointTwo.curved.value,factor);
        group->uiPoints.addCurvedPoint(x,y, val, {cx, cy, cval}, pointOne.isEdge);
        // harder
      }
    }
    // clang-format on
    group->uiPoints.updateNeighbours();
    Audio::WaveTableCreator::generate(group->uiPoints.points, group.get());
    waveLib->addGroup(group);
  }
  waveLib->remove((long)end);
  waveLib->addGroup(endG);
}
} // namespace VeNo::Utils