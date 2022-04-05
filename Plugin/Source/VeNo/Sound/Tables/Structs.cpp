#include "VeNo/Utils/ProfileMacros.h"
#include "VeNo/Utils/Timer.h"

#include <VUtils/Curve.h>
#include <VUtils/Logging.h>
#include <VUtils/Math.h>
#include <VUtils/StringUtils.h>
#include <VeNo/Sound/Tables/Structs.h>
#include <VeNo/Utils/FFT.h>
#include <cmath>
#include <limits>
#include <sstream>

namespace VeNo::Audio {
Point &Point::operator<<(const std::string &_string) {
  auto values = VUtils::StringUtils::split(_string, "|");
  if (values.size() != 3)
    return *this;
  x = (float)VUtils::StringUtils::toNumber(values[0], x);
  y = (float)VUtils::StringUtils::toNumber(values[1], y);
  value = (float)VUtils::StringUtils::toNumber(values[2], value);
  return *this;
}

std::string WavePoint::to_string() const {
  std::ostringstream str{};
  str.precision(std::numeric_limits<double>::max_digits10);
  str << "data:{" << data << "};";
  str << "isEdge:" << isEdge << ";bezier:" << bezier << ";";
  str << "curve:{" << curved << "}";
  return str.str();
}

void WavePoint::from_string(const std::string &string) {
  auto properties = VUtils::StringUtils::split(string, ";");
  for (auto &property : properties) {
    auto propSplit = VUtils::StringUtils::split(property, ":");
    auto &name = propSplit[0];
    if (propSplit.size() == 1)
      continue;
    auto &value = propSplit[1];
    if (name == "data") {
      data << value.substr(1, value.size() - 1);
    } else if (name == "isEdge") {
      isEdge = value == "1";
    } else if (name == "bezier") {
      bezier = value == "1";
    } else if (name == "curve") {
      curved << value.substr(1, value.size() - 1);
    } else {
      ERR("{} is not a property of WavePoint", name);
    }
  }
}

Wave::~Wave() { delete[] items; }
WaveTableGroup::~WaveTableGroup() { delete[] items; }

size_t findNextValidSpot(const float* items, int size, int idx, int direction) {
  int startIdx = idx;
  do {
    idx = idx + direction;
    idx = idx < 0 ? size - 1 : idx > size ? 0 : idx;
    // exit condition... we reached the same spot again
    if (startIdx == idx) {
      break;
    }
  } while(items[idx] != -1000);
  return (size_t)idx;
}

WaveGeneratorData WaveGenerator::createArray(Vector<WavePoint> &inPoints,
                                             size_t len) {
  VENO_PROFILE_FUNCTION();
  auto *items = new float[len+1];
  for (size_t i = 0; i < len+1; ++i)
    items[i] = -1000; // invalid value!

  for (auto &point : inPoints) {
    auto *next = point.next;
    if (!next)
      next = &inPoints[0];

    auto end = size_t((next->data.x) * (double)len);
    if (next->data.x == 0)
      end = len;

    auto start = size_t((point.data.x) * (double)len);
    size_t diff = end - start;
    if (point.bezier) {
      size_t times = len * 2;
      double midX = VUtils::Math::map(point.curved.x, point.data.x, next->data.x, 0, 1);
      for (size_t ii = 0; ii < times; ++ii) {
        double factor = (double)ii / (double)times;
        auto curve = VUtils::Curve::bezierCurve({0, point.data.value},
                                                {midX, point.curved.value},
                                                {1, next->data.value}, factor);
        auto pointX = (size_t)std::round((double)diff * curve.x);
        auto position = std::clamp(start + pointX, start, end);
        items[position] = (float)VUtils::Math::clamp(curve.y, -1, 1) * -1;
      }
    } else {
      for (size_t ii = 0; ii < diff; ++ii) {
        double factor = (double)ii / (double)diff;
        double value = VUtils::Math::lerp(point.data.value, next->data.value, factor);
        items[start + ii] = (float)VUtils::Math::clamp(value, -1, 1) * -1;
      }
    }
  }
  items[len] = items[0];
  size_t invalidSpots = 0;
  {
    Timer timer;
    for (size_t i = 0; i <= len; ++i) {
      if (items[i] == -1000) {
        invalidSpots++;
        size_t prev = findNextValidSpot(items, (int)len, (int)i, -1);
        size_t next = findNextValidSpot(items, (int)len, (int)i, 1);
        if (prev == i || next == i) {
          // we found no valid spot trigger error and break
          ERR("Found no valid spot...");
          break;
        }
        items[i] = (float)VUtils::Math::lerp(items[prev], items[next], 0.5);
      }
    }
    if (invalidSpots > 0) {
      DBGN("Time needed: {}", timer.elapsedMillis());
      DBGN("Found {} Invalid Spots that are fixed", invalidSpots);
    }
  }
  auto fft = VeNo::Utils::FFT(order_lookup(len));
  fft.prepare(items, len);
  fft.forward();
  fft.reverse();
  delete[] items;
  return {fft.data(), len};
}

int WaveGenerator::order_lookup(size_t len) {
  switch (len) {
  case 65536: return 16;
  case 32768: return 15;
  case 16384: return 14;
  case 8192: return 13;
  case 4096: return 12;
  case 2048: return 11;
  case 1024: return 10;
  case 512: return 9;
  case 256: return 8;
  case 128: return 7;
  case 64: return 6;
  case 32: return 5;
  default: return 1;
  }
}


std::string WaveUIPoints::to_string() {
  std::stringstream stream;
  for (auto &point : points) {
    stream << point.to_string() << "||";
  }
  return stream.str();
}
void WaveUIPoints::from_string(const std::string &data) {
  auto split = VUtils::StringUtils::split(data, "||");
  points.clear();
  for (auto &item : split) {
    if (item.empty())
      break;
    WavePoint wavePoint;
    wavePoint.from_string(item);
    points.push_back(wavePoint);
  }
}

void WaveUIPoints::addCurvedPoint(float x, float y, float val, Point point,
                                  bool isEdge) {
  points.push_back({});
  auto &last = points.back();
  last.data.x = x;
  last.data.y = y;
  last.data.value = val;
  last.isEdge = isEdge;
  last.bezier = true;
  last.curved.x = point.x;
  last.curved.y = point.y;
  last.curved.value = point.value;
}

void WaveUIPoints::addPoint(float x, float y, float val, bool isEdge) {
  points.push_back({});
  auto &last = points.back();
  last.data.x = x;
  last.data.y = y;
  last.data.value = val;
  last.isEdge = isEdge;
}
void WaveUIPoints::updateNeighbours() {
  for (size_t i = 0; i < points.size(); i++) {
    auto &current = points[i];
    if (i > 0)
      current.previous = &points[i - 1];
    else
      current.previous = nullptr;
    if (i < points.size() - 1)
      current.next = &points[i + 1];
    else
      current.next = nullptr;
  }
}
void WaveUIPoints::updateCurved() {
  for (auto &point : points) {
    if (point.bezier && point.curved.value == -500 && point.next) {
      point.curved.x = (point.data.x + point.next->data.x) / 2;
      point.curved.y = (point.data.y + point.next->data.y) / 2;
      point.curved.value = (point.data.value + point.next->data.value) / 2;
    }
  }
}
} // namespace VeNo::Audio