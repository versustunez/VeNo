#pragma once

#include <atomic>
#include <cassert>
#include <condition_variable>
#include <optional>
#include <queue>
#include <utility>

namespace VUtils {
template <typename T, typename S> struct SafeMap {
  explicit SafeMap(size_t maxSize = -1UL) : m_maxSize(maxSize), m_end(false){};

  bool add(const T &t, S &x);

  bool add(T &&t, S &&x);

  void remove(T t);

  void clear();

  bool has(T t);

  S &get(T t);

  int size();

private:
  std::unordered_map<T, S> m_map{};
  std::mutex m_mtx{};
  std::condition_variable m_cvFull{};
  const size_t m_maxSize{};
  std::atomic<bool> m_end{};
};

template <typename T, typename S> bool SafeMap<T, S>::add(const T &t, S &x) {
  std::unique_lock<std::mutex> lck(m_mtx);
  while (m_map.size() == m_maxSize && !m_end) {
    return false;
  }
  assert(!m_end);
  m_map.emplace(t, std::move(x));
  return true;
}

template <typename T, typename S> bool SafeMap<T, S>::add(T &&t, S &&x) {
  std::unique_lock<std::mutex> lck(m_mtx);
  while (m_map.size() == m_maxSize && !m_end)
    return false;
  assert(!m_end);
  m_map.push(std::move(t));
  return true;
}

template <typename T, typename S> void SafeMap<T, S>::clear() {
  std::unique_lock<std::mutex> lck(m_mtx);
  std::unordered_map<T, S> empty;
  std::swap(m_map, empty);
  m_cvFull.notify_all();
}
template <typename T, typename S> void SafeMap<T, S>::remove(T t) {
  std::unique_lock<std::mutex> lck(m_mtx);
  if (m_map.empty() || m_end)
    return;
  if (m_map.contains(t)) {
    m_map.erase(t);
  }
  m_cvFull.notify_one();
}

template <typename T, typename S> int SafeMap<T, S>::size() {
  return m_map.size();
}

template <typename T, typename S> bool SafeMap<T, S>::has(T t) {
  std::unique_lock<std::mutex> lck(m_mtx);
  return m_map.contains(t);
}

template <typename T, typename S> S &SafeMap<T, S>::get(T t) {
  std::unique_lock<std::mutex> lck(m_mtx);
  return m_map[t];
}
} // namespace VUtils