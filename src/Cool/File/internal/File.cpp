#include "../File.h"

#include <sys/stat.h>
#include <fstream>
#include <streambuf>

namespace Cool {

std::string File::_root_dir;
#ifdef DEBUG
bool File::_root_dir_is_initialized = false;
#endif

bool File::exists(std::string_view file_path) {
    struct stat buffer;
    return (stat(file_path.data(), &buffer) == 0);
}

std::string File::absolute_path(std::string_view file_path) {
    return std::filesystem::absolute(file_path).string();
}

std::string File::file_name(const std::string& file_path) {
    auto pos = file_path.find_last_of("/\\") + 1;
    return file_path.substr(pos, file_path.size() - pos);
}

std::string File::file_name_without_extension(const std::string& file_path) {
    return whithout_extension(file_name(file_path));
}

std::string File::extension(const std::string& file_path) {
    auto pos = file_path.find_last_of(".");
    if (pos < file_path.size()) {
        return file_path.substr(pos, file_path.size());
    }
    else {
        return "";
    }
}

std::string File::whithout_extension(const std::string& file_path) {
    auto pos = file_path.find_last_of(".");
    if (pos < file_path.size()) {
        return file_path.substr(0, pos);
    }
    return file_path;
}

std::string File::whithout_file_name(const std::string& file_path) {
    if (file_path.find_last_of(".") < file_path.size()) { // There is a "." of an extension, so the thing after the last "/" must be a file name
        auto pos = file_path.find_last_of("/\\");
        return file_path.substr(0, pos);
    }
    return file_path;
}

void File::to_string(std::string_view file_path, std::string* dst) {
    // Thanks to https://stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring
    std::ifstream stream(file_path);
    if (!stream.is_open()) {
        Log::ToUser::warn("File::to_string","Failed to open file : \"{}\"", file_path);
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

bool File::create_folders_if_they_dont_exist(std::string_view folder_path) {
    if (!exists(folder_path)) {
        try {
            std::filesystem::create_directories(folder_path);
            return true;
        }
        catch (std::exception e) {
            Log::ToUser::warn("File::create_folders_if_they_dont_exist","Failed :\n{}", e.what());
            return false;
        }
    }
    return true;
}

bool File::create_folders_for_file_if_they_dont_exist(std::string_view file_path) {
    return create_folders_if_they_dont_exist(whithout_file_name(std::string(file_path)));
}

void File::initialize_root_dir(std::string_view path) {
#ifdef DEBUG
    assert(!_root_dir_is_initialized);
    _root_dir_is_initialized = true;
#endif
    _root_dir = path;
    Log::info("[File::initialize_root_dir] \"{}\" is the root directory", _root_dir);
}

const std::string& File::root_dir() {
#ifdef DEBUG
    assert(_root_dir_is_initialized);
#endif
    return _root_dir;
}

} // namespace Cool