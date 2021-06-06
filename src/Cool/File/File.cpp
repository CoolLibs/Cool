#include "File.h"

#include <sys/stat.h>
#include <fstream>
#include <streambuf>

namespace Cool {

std::string File::_root_dir;
#ifndef NDEBUG
bool File::_root_dir_is_initialized = false;
#endif

bool File::Exists(std::string_view filepath) {
    struct stat buffer;
    return (stat(filepath.data(), &buffer) == 0);
}

std::string File::AbsolutePath(std::string_view filepath) {
    return std::filesystem::absolute(filepath).string();
}

std::string File::FileName(const std::string& filepath) {
    auto pos = filepath.find_last_of("/\\") + 1;
    return filepath.substr(pos, filepath.size() - pos);
}

std::string File::FileNameWithoutExtension(const std::string& filepath) {
    return WhithoutFileExtension(FileName(filepath));
}

std::string File::FileExtension(const std::string& filepath) {
    auto pos = filepath.find_last_of(".");
    if (pos < filepath.size())
        return filepath.substr(pos, filepath.size());
    else
        return "";
}

std::string File::WhithoutFileExtension(const std::string& filepath) {
    auto pos = filepath.find_last_of(".");
    if (pos < filepath.size())
        return filepath.substr(0, pos);
    return filepath;
}

std::string File::WhithoutFileName(const std::string& filepath) {
    if (filepath.find_last_of(".") < filepath.size()) { // There is a "." of an extension, so the thing after the last "/" must be a file name
        auto pos = filepath.find_last_of("/\\");
        return filepath.substr(0, pos);
    }
    return filepath;
}

void File::ToString(std::string_view filepath, std::string* dst) {
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

bool File::CreateFoldersIfDoesntExist(std::string_view folderPath) {
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

bool File::CreateFoldersForFileIfDoesntExist(std::string_view folderAndFilePath) {
    return CreateFoldersIfDoesntExist(WhithoutFileName(std::string(folderAndFilePath)).c_str());
}

void File::initialize_root_dir(std::string_view path) {
#ifndef NDEBUG
    assert(!_root_dir_is_initialized);
    _root_dir_is_initialized = true;
#endif
    _root_dir = path;
    Log::Info("Initialized File::root_dir as \"{}\"", _root_dir);
}

const std::string& File::root_dir() {
#ifndef NDEBUG
    assert(_root_dir_is_initialized);
#endif
    return _root_dir;
}

} // namespace Cool