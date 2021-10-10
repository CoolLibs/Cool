#include "FileWatcher.h"
#include <Cool/File/File.h>

namespace Cool {

static bool is_valid_file_path(std::filesystem::path path)
{
    return File::exists(path.string()) && !std::filesystem::is_directory(path);
}

FileWatcher::FileWatcher(std::filesystem::path                 path,
                         std::function<void(std::string_view)> on_file_changed_cb,
                         std::function<void(std::string_view)> on_path_invalid_cb,
                         float                                 delay_between_checks)
    : _path{path}
    , _time_of_last_change{std::filesystem::last_write_time(_path)}
    , _on_file_changed{on_file_changed_cb}
    , _on_path_invalid{on_path_invalid_cb}
    , _delay_between_checks{delay_between_checks}
{
}

void FileWatcher::update() const
{
    std::visit([&](auto&& validity) { update_validity(validity, is_valid_file_path(_path)); }, _path_validity);
    if (path_is_valid()) {
        using clock = std::chrono::steady_clock;
        // Init static variable (it is static so that we don't need to include chrono in the header file)
        static auto _time_of_last_check = clock::now();
        // Wait for delay between checks
        const auto now          = clock::now();
        const auto elapsed_time = std::chrono::duration<float>{now - _time_of_last_check};
        if (elapsed_time.count() > _delay_between_checks) {
            _time_of_last_check = now;
            // Check file was updated since last check
            const auto last_change = std::filesystem::last_write_time(_path);
            if (last_change != _time_of_last_change) {
                on_file_changed();
            }
        }
    }
}

void FileWatcher::on_file_changed() const
{
    _path_validity       = Valid{};
    _time_of_last_change = std::filesystem::last_write_time(_path);
    _on_file_changed(_path.string());
}

void FileWatcher::on_path_invalid() const
{
    _path_validity = Invalid{};
    _on_path_invalid(_path.string());
}

void FileWatcher::set_path(std::string_view path)
{
    _path          = path;
    _path_validity = Unknown{};
}

void FileWatcher::update_validity(Valid, bool is_valid) const
{
    if (!is_valid) {
        on_path_invalid();
    }
}

void FileWatcher::update_validity(Invalid, bool is_valid) const
{
    if (is_valid) {
        on_file_changed();
    }
}

void FileWatcher::update_validity(Unknown, bool is_valid) const
{
    if (is_valid) {
        on_file_changed();
    }
    else {
        on_path_invalid();
    }
}

} // namespace Cool