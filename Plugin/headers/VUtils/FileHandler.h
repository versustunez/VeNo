#pragma once

#include <string>

namespace VUtils
{
class FileHandler
{
public:
    static bool fileExists (const std::string& fileName);
    static bool isDirectory (const std::string& fileName);
    static std::string readFile (const std::string& fileName);
    static bool writeFile (const std::string& fileName, const std::string& content);
    static int getFileID (const std::string& fileName);
    static void closeFD (int fd);
    static std::string getExtension (const std::string& fileName);
    static long getFileSize (int fd);
    static std::string getFileName (const std::basic_string<char>& name);
    static bool createDirectoryIfNotExist (const std::basic_string<char>& fileName);
    static char* getHomeDirectory();
    static std::string getFromHomeDir (const std::basic_string<char>& path);
};
} // namespace VUtils
