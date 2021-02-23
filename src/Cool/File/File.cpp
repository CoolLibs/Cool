#include "File.h"

#include <sys/stat.h>
#include <fstream>
#include <streambuf>

namespace Cool::File {

bool Exists(std::string_view filepath) {
    struct stat buffer;
    return (stat(filepath.data(), &buffer) == 0);
}

std::string AbsolutePath(std::string_view filepath) {
    return std::filesystem::absolute(filepath).string();
}

std::string FileName(const std::string& filepath) {
    auto pos = filepath.find_last_of("/\\") + 1;
    return filepath.substr(pos, filepath.size() - pos);
}

std::string FileNameWithoutExtension(const std::string& filepath) {
    return WhithoutFileExtension(FileName(filepath));
}

std::string FileExtension(const std::string& filepath) {
    auto pos = filepath.find_last_of(".");
    if (pos < filepath.size())
        return filepath.substr(pos, filepath.size());
    else
        return "";
}

std::string WhithoutFileExtension(const std::string& filepath) {
    auto pos = filepath.find_last_of(".");
    if (pos < filepath.size())
        return filepath.substr(0, pos);
    return filepath;
}

std::string WhithoutFileName(const std::string& filepath) {
    if (filepath.find_last_of(".") < filepath.size()) { // There is a "." of an extension, so the thing after the last "/" must be a file name
        auto pos = filepath.find_last_of("/");
        return filepath.substr(0, pos);
    }
    return filepath;
}

void ToString(std::string_view filepath, std::string* dst) {
    // Thanks to https://stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring
    std::ifstream stream(filepath);
    if (!stream.is_open()) {
        Log::Warn("[MyFile::ToString] Failed to open file : '{}'", filepath);
        return;
    }
    stream.seekg(0, std::ios::end);
    dst->reserve(stream.tellg());
    stream.seekg(0, std::ios::beg);
    dst->assign(
        (std::istreambuf_iterator<char>(stream)),
        std::istreambuf_iterator<char>()
    );
    stream.close();
}

bool CreateFoldersIfDoesntExist(std::string_view folderPath) {
    if (!Exists(folderPath)) {
        try {
            std::filesystem::create_directories(folderPath);
            return true;
        }
        catch (std::exception e) {
            Log::Warn(e.what());
            return false;
        }
    }
    return true;
}

bool CreateFoldersForFileIfDoesntExist(std::string_view folderAndFilePath) {
    return CreateFoldersIfDoesntExist(WhithoutFileName(std::string(folderAndFilePath)).c_str());
}

} // namespace Cool