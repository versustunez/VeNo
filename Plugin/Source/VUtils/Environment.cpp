#include <VUtils/Environment.h>
#include <VUtils/FileHandler.h>
#include <VUtils/Logging.h>
#include <VUtils/StringUtils.h>
#include <iomanip>
#include <sstream>
#include <vector>

namespace VUtils {
Environment::Environment(const char *filename) : m_filename(filename) {
  m_env[""] = "";
}
Environment::Environment() { m_env[""] = ""; }

void Environment::loadFile() {
  DBGN("Load ENV-File: {}", m_filename.c_str());
  if (!FileHandler::fileExists(m_filename)) {
    WARN("Cannot Load Env-File {}! File not found", m_filename.c_str());
    return;
  }
  auto content = StringUtils::trimCopy(FileHandler::readFile(m_filename));
  auto lines = StringUtils::split(content, "\n");
  for (auto &line : lines) {
    if (line.empty()) {
      continue;
    }
    auto split = StringUtils::split(line, "=");
    if (split.size() >= 2) {
      m_env[StringUtils::trimCopy(split[0])] = StringUtils::trimCopy(split[1]);
    } else {
      m_env[StringUtils::trimCopy(split[0])] = "true";
    }
  }
  DBGN("Found {} Elements for Environment File {}", m_env.size(),
       m_filename.c_str());
}

std::string &Environment::getEnv(const std::string &name, std::string def) {
  if (m_env.find(name) != m_env.end()) {
    return m_env[name];
  }
  auto *val = std::getenv(std::string(m_prefix + name).c_str());
  if (val) {
    m_env[name] = std::string(val);
    return m_env[name];
  }
  if (def.empty()) {
    return m_env[""];
  }
  m_env[name] = std::move(def);
  return m_env[name];
}

bool Environment::hasEnv(const std::string &name) {
  return m_env.find(name) != m_env.end() ||
         std::getenv(name.c_str()) != nullptr;
}

int Environment::getAsInt(const std::string &name, int def) {
  return (int)getAsDouble(name, def);
}

double Environment::getAsDouble(const std::string &name, double def) {
  char *end;
  auto *v = getEnv(name, "").c_str();
  auto val = (double)std::strtod(v, &end);
  if (end == v) {
    setNumber(name.c_str(), def);
    return def;
  }
  return val;
}

bool Environment::getAsBool(const std::string &name) {
  return getEnv(name, "false") == "true";
}

void Environment::setPrefix(std::string prefix) {
  m_prefix = std::move(prefix);
}

bool Environment::saveFile() {
  // override file if not exists!
  std::stringstream stream{};
  stream << std::setprecision(4);
  for (auto &element : m_env) {
    if (element.first.empty())
      continue;
    stream << element.first << "=" << element.second << "\n";
  }
  if (!FileHandler::createDirectoryIfNotExist(m_filename)) {
    ERR("Directory not exists or cannot create for: {}", m_filename.c_str());
    return false;
  }
  if (!FileHandler::writeFile(m_filename, stream.str())) {
    WARN("Cannot Save Env-File {}! Write failed", m_filename.c_str());
    return false;
  }
  DBGN("Saved file to: {}", m_filename.c_str());
  return true;
}

void Environment::setFile(const char *filename) { m_filename = filename; }

void Environment::set(const char *name, const char *value) {
  m_env[name] = value;
}

// Small hack that set numbers to max precision
void Environment::setNumber(const char *name, double value) {
  int newValue = (int)value;
  std::ostringstream out;

  if (value != newValue) {
    out.precision(4);
  } else {
    out.precision(0);
  }
  out << std::fixed << value;
  m_env[name] = out.str();
}
} // namespace VUtils
