#include "FileWatcher.h"
#include <Cool/File/File.h>

namespace Cool {

static bool is_valid_file_path(std::filesystem::path path)
{
    return File::exists(path.string()) && !std::filesystem::is_directory(path);
}

static auto time_of_last_change(std::filesystem::path path) -> std::filesystem::file_time_type
{
    try {
        return std::filesystem::last_write_time(path);
    }
    catch (...) {
        return {};
    }
}

FileWatcher::FileWatcher(std::filesystem::path path, FileWatcher_Config config)
    : _path{path}
    , _time_of_last_change{time_of_last_change(path)}
    , _config{config}
{
}

void FileWatcher::update(FileWatcher_Callbacks callbacks) const
{
    std::visit([&](auto&& validity) { update_validity(validity, is_valid_file_path(_path), callbacks); }, _path_validity);
    if (path_is_valid()) {
        using clock = std::chrono::steady_clock;
        // Init static variable (it is static so that we don't need to include chrono in the header file)
        static auto _time_of_last_check = clock::now();
        // Wait for delay between checks
        const auto now          = clock::now();
        const auto elapsed_time = std::chrono::duration<float>{now - _time_of_last_check};
        if (elapsed_time.count() > _config.delay_between_checks) {
            _time_of_last_check = now;
            // Check file was updated since last check
            const auto last_change = time_of_last_change(_path);
            if (last_change != _time_of_last_change) {
                on_file_changed(callbacks);
            }
        }
    }
}

void FileWatcher::on_file_changed(FileWatcher_Callbacks callbacks) const
{
    _path_validity       = Valid{};
    _time_of_last_change = time_of_last_change(_path);
    callbacks.on_file_changed(_path.string());
}

void FileWatcher::on_path_invalid(FileWatcher_Callbacks callbacks) const
{
    _path_validity = Invalid{};
    callbacks.on_path_invalid(_path.string());
}

void FileWatcher::set_path(std::string_view path)
{
    _path          = path;
    _path_validity = Unknown{};
}

void FileWatcher::update_validity(Valid, bool is_valid, FileWatcher_Callbacks callbacks) const
{
    if (!is_valid) {
        on_path_invalid(callbacks);
    }
}

void FileWatcher::update_validity(Invalid, bool is_valid, FileWatcher_Callbacks callbacks) const
{
    if (is_valid) {
        on_file_changed(callbacks);
    }
}

void FileWatcher::update_validity(Unknown, bool is_valid, FileWatcher_Callbacks callbacks) const
{
    if (is_valid) {
        on_file_changed(callbacks);
    }
    else {
        on_path_invalid(callbacks);
    }
}

} // namespace Cool