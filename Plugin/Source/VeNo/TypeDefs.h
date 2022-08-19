#pragma once

#include <memory>
#include <mutex>
#include <queue>
#include <string>
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
using VString = std::string;

template <typename T, typename S> using Map = std::unordered_map<T, S>;
template <typename T> using Queue = std::queue<T>;
template <typename T> using Dequeue = std::deque<T>;
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

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef size_t s64;
typedef size_t InstanceID;
} // namespace VeNo
