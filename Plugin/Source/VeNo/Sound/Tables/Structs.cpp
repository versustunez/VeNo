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
      ERR("%s is not a property of WavePoint", name.c_str());
    }
  }
}

Wave::~Wave() { delete items; }
WaveTableGroup::~WaveTableGroup() { delete items; }

WaveGeneratorData WaveGenerator::createArray(std::vector<WavePoint> &inPoints,
                                             size_t len) {
  auto *items = new float[len];
  for (auto &point : inPoints) {
    auto *next = point.next;
    if (!next)
      next = &inPoints[0];

    auto end = size_t((next->data.x) * (double)len);
    if (next->data.x == 0)
      end = len;

    auto start = size_t((point.data.x) * (double)len);
    size_t diff = end - start;
    if (point.bezier && point.next) {
      size_t times = len * 2;
      double midX =
          VUtils::Math::map(point.curved.x, point.data.x, next->data.x, 0, 1);
      for (size_t ii = 0; ii < times; ++ii) {
        double factor = (double)ii / (double)times;
        auto curve = VUtils::Curve::bezierCurve({0, point.data.value},
                                                {midX, point.curved.value},
                                                {1, next->data.value}, factor);
        auto pointX = size_t(std::round((double)diff * curve.x));
        auto position = std::clamp(start + pointX, start, end);
        items[position] = (float)VUtils::Math::clamp(curve.y, -1, 1) * -1;
      }
    } else {
      for (size_t ii = 0; ii < diff; ++ii) {
        double factor = (double)ii / (double)diff;
        double value =
            VUtils::Math::lerp(point.data.value, next->data.value, factor);
        items[start + ii] = (float)VUtils::Math::clamp(value, -1, 1) * -1;
      }
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
} // namespace VeNo::Audio