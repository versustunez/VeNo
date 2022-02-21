#pragma once

#include <atomic>
#include <cassert>
#include <condition_variable>
#include <optional>
#include <queue>
#include <utility>

namespace VUtils {
template <typename T> struct SafeQueue {
  explicit SafeQueue(size_t maxSize = -1UL)
      : m_maxSize(maxSize),
        m_end(false){};

  void push(const T &t);

  void push(T &&t);

  void close();

  void clear();

  std::optional<T> pop();

  std::optional<T> waitAndPop();

  bool isClosed();

  int size();

  void flush();
  std::queue<T> getRemaining();

private:
  std::queue<T> m_que;
  std::mutex m_mtx;
  std::condition_variable m_cvEmpty, m_cvFull;
  const size_t m_maxSize;
  std::atomic<bool> m_end;
};

template <typename T> void SafeQueue<T>::push(const T &t) {
  std::unique_lock<std::mutex> lck(m_mtx);
  while (m_que.size() == m_maxSize && !m_end) {
    // we dont wait! we return false because queue is full...
    m_cvFull.wait(lck);
  }
  if (m_end)
    return;
  m_que.push(std::move(t));
  m_cvEmpty.notify_one();
}

template <typename T> void SafeQueue<T>::push(T &&t) {
  std::unique_lock<std::mutex> lck(m_mtx);
  while (m_que.size() == m_maxSize && !m_end)
    m_cvFull.wait(lck);
  assert(!m_end);
  m_que.push(std::move(t));
  m_cvEmpty.notify_one();
}

template <typename T> void SafeQueue<T>::close() {
  m_end = true;
  std::lock_guard<std::mutex> lck(m_mtx);
  m_cvEmpty.notify_all();
  m_cvFull.notify_all();
}

template <typename T> std::optional<T> SafeQueue<T>::pop() {
  std::unique_lock<std::mutex> lck(m_mtx);
  if (m_que.empty() || m_end)
    return {};
  T t = std::move(m_que.front());
  m_que.pop();
  m_cvFull.notify_one();
  return t;
}

template <typename T> std::optional<T> SafeQueue<T>::waitAndPop() {
  std::unique_lock<std::mutex> lck(m_mtx);
  while (m_que.empty() && !m_end)
    m_cvEmpty.wait(lck);
  if (m_que.empty() || m_end)
    return {};
  T t = std::move(m_que.front());
  m_que.pop();
  m_cvFull.notify_one();
  return t;
}

template <typename T> void SafeQueue<T>::clear() {
  std::unique_lock<std::mutex> lck(m_mtx);
  std::queue<T> empty;
  std::swap(m_que, empty);
  m_cvEmpty.notify_all();
  m_cvFull.notify_all();
}

template <typename T> bool SafeQueue<T>::isClosed() { return m_end; }

template <typename T> int SafeQueue<T>::size() { return m_que.size(); }
template <typename T> void SafeQueue<T>::flush() {
  while (!m_que.empty()) {
    m_cvEmpty.notify_one();
  }
  m_end = true;
  m_cvEmpty.notify_all();
}
template <typename T> std::queue<T> SafeQueue<T>::getRemaining() {
  return m_que;
}
} // namespace VUtils
