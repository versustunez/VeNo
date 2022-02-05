#pragma once

#include <memory>
#include <mutex>
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

} // namespace VeNo
