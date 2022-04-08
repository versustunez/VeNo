#pragma once

#include <VeNo/TypeDefs.h>
#include <functional>

namespace VeNo::Utils {
template <size_t TS> class LookupTable {
public:
  LookupTable() = default;
  explicit LookupTable(const std::function<void(float *elements, size_t size)>& initFunction) {
    initFunction(elements, size);
  }

  float operator[](int index) { return elements[index]; }

protected:
  float elements[TS]{};
  size_t size{TS};
};
} // namespace VeNo::Utils