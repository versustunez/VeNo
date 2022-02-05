#include <VUtils/Logging.h>
#include <VeNo/Utils/Profile.h>

#include <utility>

namespace VeNo::Perf {
void Profiler::begin(
    const std::string &name, const std::string &filepath = "results.json") {
  std::lock_guard lock(m_mutex);
  if (m_currentSession)
    internalEnd();
  std::string resolvedPath = VUtils::FileHandler::resolve(filepath);
  VUtils::FileHandler::createParentDirectories(resolvedPath);
  m_outputStream.open(resolvedPath, std::fstream::out);
  m_outputStream.flush();
  if (m_outputStream.is_open()) {
    LOG("Started Profile File: %s (%s)", name.c_str(), resolvedPath.c_str());
    m_currentSession = new Session({name});
    writeHeader();
  } else
    ERR("Profiler cannot open Profiler File %s", resolvedPath.c_str());
}
void Profiler::end() {
  std::lock_guard lock(m_mutex);
  internalEnd();
}
void Profiler::write(const ProfileResult &result) {
  std::stringstream json;

  json << std::setprecision(3) << std::fixed;
  json << ",{";
  json << R"("cat":"function",)";
  json << R"("dur":)" << (result.elapsedTime.count()) << ',';
  json << R"("name":")" << result.name << R"(",)";
  json << R"("ph":"X",)";
  json << R"("pid":0,)";
  json << R"("tid":)" << result.threadID << ',';
  json << R"("ts":)" << result.start.count();
  json << '}';

  std::lock_guard lock(m_mutex);
  if (m_currentSession) {
    m_outputStream << json.str();
    m_outputStream.flush();
  }
}

void Profiler::internalEnd() {
  if (m_currentSession) {
    writeFooter();
    m_outputStream.close();
    delete m_currentSession;
    m_currentSession = nullptr;
  }
}

void Profiler::writeHeader() {
  m_outputStream << R"({"otherData": {},"traceEvents":[{})";
  m_outputStream.flush();
}

void Profiler::writeFooter() {
  m_outputStream << "]}";
  m_outputStream.flush();
}

ProfilerTimer::ProfilerTimer(std::string name)
    : m_name(std::move(name)), m_stopped(false) {
  m_startTimepoint = std::chrono::steady_clock::now();
}

ProfilerTimer::~ProfilerTimer() {
  if (!m_stopped)
    stop();
}

void ProfilerTimer::stop() {
  auto endTimePoint = std::chrono::steady_clock::now();
  auto highResStart =
      FloatingPointMicroseconds{m_startTimepoint.time_since_epoch()};
  auto elapsedTime =
      std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint)
          .time_since_epoch() -
      std::chrono::time_point_cast<std::chrono::microseconds>(m_startTimepoint)
          .time_since_epoch();

  Profiler::the().write(
      {m_name, highResStart, elapsedTime, std::this_thread::get_id()});

  m_stopped = true;
}
} // namespace VeNo::Perf
