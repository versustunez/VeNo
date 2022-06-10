#include "VeNo/Core/Config.h"
#include "VeNo/Utils/ProfileMacros.h"
#include "VeNo/Utils/Timer.h"
#include "VeNo/Utils/Math.h"

#include <VUtils/Curve.h>
#include <VUtils/Logging.h>
#include <VUtils/Math.h>
#include <VUtils/StringUtils.h>
#include <VeNo/Sound/Tables/Structs.h>
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

WaveGeneratorData WaveGenerator::createArray(Vector<WavePoint> &inPoints,
                                             size_t len) {
  VENO_PROFILE_FUNCTION();
  auto *items = new float[len + 1];
  for (size_t i = 0; i < len; ++i)
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
      size_t times = diff;
      double midX =
          VUtils::Math::map(point.curved.x, point.data.x, next->data.x, 0, 1);
      for (size_t ii = 0; ii < times; ++ii) {
        double factor = (double)ii / (double)times;
        auto curve = VUtils::Curve::bezierCurve({0, point.data.value},
                                                {midX, point.curved.value},
                                                {1, next->data.value}, powerScale(factor, 0.0));
        items[start+ii] = (float)VUtils::Math::clamp(curve.y, -1, 1) * -1;
      }
    } else {
      for (size_t ii = 0; ii < diff; ++ii) {
        double factor = (double)ii / (double)diff;
        double value = VUtils::Math::lerp(point.data.value, next->data.value, powerScale(factor, 0.0));
        items[start + ii] = (float)VUtils::Math::clamp(value, -1, 1) * -1;
      }
    }
  }
  double state = 0;
  double const cutoff_frequency = 1.0;
  const double gain = cutoff_frequency / (2.0 * V_PI * Core::Config::get().sampleRate);
  for(size_t i = 0; i < len; ++i) {
    double retval = items[i] - state;
    state += gain * retval;
    items[i] = retval;
  }
  items[len] = items[0];
  return {items, len};
}

double WaveGenerator::powerScale(double value, double power) {
  static constexpr double kMinPower = 0.01f;

  if (fabs(power) < kMinPower)
    return value;

  double numerator = exp(power * value) - 1.0f;
  double denominator = exp(power) - 1.0f;
  return numerator / denominator;
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
  updateXCurvedOnAdd();
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

void WaveUIPoints::updateXCurvedOnAdd() {
  for (auto &point : points) {
    if (point.bezier && point.next)
      point.curved.x = (point.data.x + point.next->data.x) / 2;
  }
}
} // namespace VeNo::Audio