#include <VUtils/FileHandler.h>
#include <VUtils/Logging.h>
#include <VUtils/OsCheck.h>
#include <fcntl.h>
#include <filesystem>
#include <fstream>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>

#ifndef isWin

#include <pwd.h>

#endif
namespace VUtils
{
bool FileHandler::fileExists (const std::string& fileName)
{
    return std::filesystem::exists (fileName);
}

std::string FileHandler::readFile (const std::string& fileName)
{
    std::ifstream ifs (fileName.c_str(),
                       std::ios::in | std::ios::binary | std::ios::ate);
    std::ifstream::pos_type fileSize = ifs.tellg();
    ifs.seekg (0, std::ios::beg);

    std::vector<char> bytes ((unsigned long) fileSize);
    ifs.read (bytes.data(), fileSize);

    return std::string (bytes.data(), (unsigned long) fileSize);
}

bool FileHandler::writeFile (const std::string& fileName,
                             const std::string& content)
{
    try
    {
        std::ofstream ofs (fileName);
        ofs << content;
        ofs.close();
    }
    catch (std::exception& e)
    {
        ERR ("Save Failed: %s", e.what());
        return false;
    }
    return true;
}
bool FileHandler::isDirectory (const std::string& fileName)
{
    return std::filesystem::is_directory (fileName);
}

std::string FileHandler::getExtension (const std::string& fileName)
{
    auto ext = std::filesystem::path (fileName).extension().string();
    if (ext.empty())
        return ext;
    return ext.erase (0, 1);
}

int FileHandler::getFileID (const std::string& fileName)
{
    return open (fileName.c_str(), O_RDONLY);
}

long FileHandler::getFileSize (int fd)
{
    struct stat stat_buf
    {
    };
    int rc = fstat (fd, &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
}

void FileHandler::closeFD (int fd) { close (fd); }

std::string FileHandler::getFileName (const std::basic_string<char>& name)
{
    auto p = std::filesystem::path (name);
    return p.filename().replace_extension ("");
}

bool FileHandler::createDirectoryIfNotExist (
    const std::basic_string<char>& fileName)
{
    auto p = std::filesystem::path (fileName);
    if (! p.has_parent_path())
    {
        WARN ("Path has no Parent Path")
        return false;
    }
    if (FileHandler::isDirectory (p.parent_path()))
        return true;

    return std::filesystem::create_directories (p.parent_path());
}
char* FileHandler::getHomeDirectory()
{
    char* homedir;
#ifdef isWin
    homedir = getenv ("USERPROFILE");
    if (homedir == nullptr)
        homedir = getenv ("HOMEDRIVE");
    if (homedir == nullptr)
        ERR ("Hell NO! Here is something complete wrong!")
#endif
#if isLinux || isBSD || isMAC
    if ((homedir = getenv ("HOME")) == nullptr)
    {
        auto x = getpwuid (getuid());
        if (x)
            homedir = x->pw_dir;
    }
#endif
    return homedir;
}
std::string FileHandler::getFromHomeDir (const std::basic_string<char>& path)
{
    auto homeDir = getHomeDirectory();
    if (homeDir == nullptr)
    {
        WARN ("HomeDir not found return original Path... please handle with care!")
        return path;
    }
    return std::string (homeDir + path);
}
} // namespace VUtils