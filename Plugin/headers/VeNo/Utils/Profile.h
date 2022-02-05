#pragma once
#include <VeNo/TypeDefs.h>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>

// Inspired from Hazel :)
namespace VeNo::Perf {
using FloatingPointMicroseconds = std::chrono::duration<double, std::micro>;

struct ProfileResult {
  std::string name;

  FloatingPointMicroseconds start;
  std::chrono::microseconds elapsedTime;
  std::thread::id threadID;
};

struct Session {
  std::string name;
};

class Profiler {
public:
  static Profiler &the() {
    static Profiler instance;
    return instance;
  }

public:
  Profiler(const Profiler &) = delete;
  Profiler(Profiler &&) = delete;

  void begin(const std::string &name, const std::string &filepath);
  void end();
  void write(const ProfileResult &result);

private:
  Profiler() = default;
  ~Profiler() { end(); };
  void internalEnd();
  void writeHeader();
  void writeFooter();

private:
  std::mutex m_mutex;
  Session *m_currentSession{nullptr};
  std::ofstream m_outputStream;
};

class ProfilerTimer {
public:
  ProfilerTimer(std::string name);
  ~ProfilerTimer();
  void stop();

private:
  std::string m_name;
  std::chrono::time_point<std::chrono::steady_clock> m_startTimepoint;
  bool m_stopped;
};

} // namespace VeNo::Perf
