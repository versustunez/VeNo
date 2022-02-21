#pragma once

#include <memory>
#include <mutex>
#include <queue>
#include <unordered_map>

namespace VeNo {
template <typename T> using Scope = std::unique_ptr<T>;
template <typename T, typename... Args>
constexpr Scope<T> CreateScope(Args &&...args) {
  return std::make_unique<T>(std::forward<Args>(args)...);
}

template <typename T> using Ref = std::shared_ptr<T>;
template <typename T, typename... Args>
constexpr Ref<T> CreateRef(Args &&...args) {
  return std::make_shared<T>(std::forward<Args>(args)...);
}

using Mutex = std::mutex;
using Guard = std::lock_guard<Mutex>;

template <typename T, typename S> using Map = std::unordered_map<T, S>;
template <typename T> using Queue = std::queue<T>;
template <typename T> using Vector = std::vector<T>;

#define NO_COPY_BASE_CONSTRUCTOR(TypeName)                                     \
public:                                                                        \
  TypeName() = default;                                                        \
                                                                               \
private:                                                                       \
  TypeName(const TypeName &) = delete;                                         \
  void operator=(const TypeName) = delete

#define NO_COPY(TypeName)                                                      \
  TypeName(const TypeName &) = delete;                                         \
  void operator=(const TypeName) = delete
} // namespace VeNo
