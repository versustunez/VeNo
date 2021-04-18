#pragma once

#include <algorithm>
#include <string>

namespace VUtils
{
class StringUtils
{
public:
    static int stringToInt (std::string& string, int def);
    static double stringToDouble (std::string& string, double def);
    static void leftTrim (std::string& s);

    static void rightTrim (std::string& s);

    static void trim (std::string& s);

    static std::string leftTrimCopy (std::string s);

    static std::string rightTrimCopy (std::string s);

    static std::string trimCopy (std::string s);

    static std::vector<std::string> split (const std::string& s,
                                           const std::string& delimiter);

    static std::string urlDecode (const std::string& url);

    static std::string urlEncode (const std::string& url);

    static std::string join (const std::vector<std::string>& vector,
                             const std::string& delimiter);

    static bool hasNullByte (int size, const char* string);
    static void toLower (std::string& str);
};
} // namespace VUtils
