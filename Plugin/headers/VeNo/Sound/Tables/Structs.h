#pragma once
#include <VeNo/TypeDefs.h>
#include <cstddef>
#include <sstream>
#include <string>

namespace VeNo::Audio {

struct Point {
  float x{0};        // normalized
  float y{0};        // normalized
  float value{-500}; // -500 is an invalid value, so we will recalculate it
  friend std::ostream &operator<<(std::ostream &out, Point const &data) {
    out << data.x << "|" << data.y << "|" << data.value;
    return out;
  }
  Point &operator<<(const std::string &_string);
  NO_COPY_BASE_CONSTRUCTOR(Point);
};

struct WavePoint {
  Point data{};
  Point curved{};
  bool isEdge{true};
  bool bezier{false};
  WavePoint *previous{nullptr};
  WavePoint *next{nullptr};

  [[nodiscard]] std::string to_string() const;
  void from_string(const std::string &);
  NO_COPY_BASE_CONSTRUCTOR(WavePoint);
};

struct Wave {
  float *items{nullptr};
  size_t len{0};
  ~Wave();
  double freq{0};
  NO_COPY_BASE_CONSTRUCTOR(Wave);
};

struct WaveTableGroup {
  ~WaveTableGroup();
  Wave *items{nullptr};
  size_t len{0};
  NO_COPY_BASE_CONSTRUCTOR(WaveTableGroup);
};
struct WaveGeneratorData {
  float *data{nullptr};
  size_t size{0};
  NO_COPY_BASE_CONSTRUCTOR(WaveGeneratorData);
};
struct WaveGenerator {
  static WaveGeneratorData createArray(std::vector<WavePoint> &inPoints,
                                       size_t len);
  static int order_lookup(size_t len);
};
} // namespace VeNo::Audio