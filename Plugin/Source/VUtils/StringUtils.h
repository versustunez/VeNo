#pragma once

#include <algorithm>
#include <string>
#include <vector>

namespace VUtils {
class StringUtils {
public:
  static int toNumber(std::string &string, int def);
  static long toNumber(std::string &string, long def);
  static double toNumber(std::string &string, double def);
  static void leftTrim(std::string &s);

  static void rightTrim(std::string &s);

  static void trim(std::string &s);

  static std::string leftTrimCopy(std::string s);

  static std::string rightTrimCopy(std::string s);

  static std::string trimCopy(std::string s);

  static std::vector<std::string>
  split(const std::string &s, const std::string &delimiter, int limit = -1);

  static std::string urlDecode(const std::string &url);

  static std::string urlEncode(const std::string &url);

  static std::string join(const std::vector<std::string> &vector,
                          const std::string &delimiter);

  static bool hasNullByte(int size, const char *string);
  static void toLower(std::string &str);
  static void toUpper(std::string &str);
  static std::string toString(double value, int precision, bool fixed = false);
};
} // namespace VUtils
