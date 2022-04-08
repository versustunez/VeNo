#pragma once

#include <string>
#include <unordered_map>

namespace VUtils {
class Environment {
public:
  Environment();
  explicit Environment(const char *filename);
  void setFile(const char *filename);
  void setPrefix(std::string prefix);
  void loadFile();
  std::string &getEnv(const std::string &name, std::string def);
  bool hasEnv(const std::string &name);
  int getAsInt(const std::string &name, int def);
  double getAsDouble(const std::string &name, double def);
  bool getAsBool(const std::string &name);
  bool saveFile();
  void set(const char *name, const char *value);
  void setNumber(const char *name, double value);

protected:
  std::unordered_map<std::string, std::string> m_env;
  std::string m_prefix = "VENV_";
  std::string m_filename;
};
} // namespace VUtils
