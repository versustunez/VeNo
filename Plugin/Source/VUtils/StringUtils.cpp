#include <VUtils/StringUtils.h>
#include <functional>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

namespace VUtils {
void StringUtils::leftTrim(std::string &s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
          }));
}

void StringUtils::rightTrim(std::string &s) {
  s.erase(std::find_if(s.rbegin(), s.rend(),
                       [](unsigned char ch) { return !std::isspace(ch); })
              .base(),
          s.end());
}

void StringUtils::trim(std::string &s) {
  leftTrim(s);
  rightTrim(s);
}

std::string StringUtils::leftTrimCopy(std::string s) {
  leftTrim(s);
  return s;
}

std::string StringUtils::rightTrimCopy(std::string s) {
  rightTrim(s);
  return s;
}

std::string StringUtils::trimCopy(std::string s) {
  trim(s);
  return s;
}

std::vector<std::string> StringUtils::split(const std::string &s,
                                            const std::string &delimiter,
                                            int limit) {
  size_t pos_start = 0, pos_end, delim_len = delimiter.length();
  std::string token;
  std::vector<std::string> res;

  while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos &&
         (limit == -1 || res.size() < (size_t)limit)) {
    token = s.substr(pos_start, pos_end - pos_start);
    pos_start = pos_end + delim_len;
    res.push_back(token);
  }

  res.push_back(s.substr(pos_start));
  return res;
}

std::string StringUtils::urlDecode(const std::string &val) {
  std::string ret;
  char ch;
  size_t i;
  unsigned int ii;
  for (i = 0; i < val.length(); i++) {
    if (int(val[i]) == 37) {
      char *pEnd;
      ii = std::strtoul(val.substr(i + 1, 2).c_str(), &pEnd, 16);
      ch = static_cast<char>(ii);
      ret += ch;
      i = i + 2;
    } else
      ret += val[i];
  }
  return (ret);
}

std::string StringUtils::urlEncode(const std::string &value) {
  std::ostringstream escaped;
  escaped.fill('0');
  escaped << std::hex;

  for (char c : value) {
    if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
      escaped << c;
      continue;
    }
    escaped << std::uppercase;
    escaped << '%' << std::setw(2) << int((unsigned char)c);
    escaped << std::nouppercase;
  }

  return escaped.str();
}

std::string StringUtils::join(const std::vector<std::string> &vector,
                              const std::string &delimiter) {
  std::stringstream string;
  for (size_t i = 0; i < vector.size(); ++i) {
    if (i != 0)
      string << delimiter;
    string << vector[i];
  }
  return string.str();
}

bool StringUtils::hasNullByte(int size, const char *string) {
  for (int i = 0; i < size; ++i) {
    if (string[i] == '\0')
      return true;
  }
  return false;
}

void StringUtils::toLower(std::string &str) {
  std::transform(str.begin(), str.end(), str.begin(), ::tolower);
}

void StringUtils::toUpper(std::string &str) {
  std::transform(str.begin(), str.end(), str.begin(), ::toupper);
}

int StringUtils::toNumber(std::string &string, int def) {
  char *pEnd;
  long val = std::strtol(string.c_str(), &pEnd, 10);
  if (pEnd == string)
    return def;
  return (int)val;
}

long StringUtils::toNumber(std::string &string, long def) {
  char *pEnd;
  long val = std::strtol(string.c_str(), &pEnd, 10);
  if (pEnd == string)
    return def;
  return val;
}

double StringUtils::toNumber(std::string &string, double def) {
  char *pEnd;
  double val = std::strtod(string.c_str(), &pEnd);
  if (pEnd == string)
    return def;
  return val;
}
std::string StringUtils::toString(double value, int precision, bool fixed) {
  std::stringstream str{};
  if (fixed) {
    str.setf(std::ios::fixed);
  }
  str.precision(precision);
  str << value;
  return str.str();
}
} // namespace VUtils