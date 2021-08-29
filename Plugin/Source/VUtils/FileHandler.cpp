#include <JuceHeader.h>
#include <VUtils/FileHandler.h>
#include <VUtils/Logging.h>
#include <fstream>
#include <vector>
namespace VUtils
{
bool FileHandler::fileExists (const std::string& fileName)
{
    return juce::File (fileName).existsAsFile();
}

std::string FileHandler::readFile (const std::string& fileName)
{
    auto file = juce::File (fileName);
    if (! file.existsAsFile())
        return "";
    return file.loadFileAsString().toStdString();
}

bool FileHandler::writeFile (const std::string& fileName,
                             const std::string& content)
{
    if (! fileExists (fileName))
        return false;
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
    auto file = juce::File (fileName);
    return file.isDirectory();
}

std::string FileHandler::getExtension (const std::string& fileName)
{
    auto file = juce::File (fileName);
    return file.getFileExtension().toStdString();
}

std::string FileHandler::getFileName (const std::basic_string<char>& name)
{
    if (name.find ("Bin::") == 0)
        return name;
    return juce::File (name).getFileNameWithoutExtension().toStdString();
}

bool FileHandler::createDirectoryIfNotExist (const std::basic_string<char>& fileName)
{
    auto file = juce::File (fileName);
    if (! file.exists())
        return file.createDirectory().wasOk();
    else
        return true;
}
} // namespace VUtils