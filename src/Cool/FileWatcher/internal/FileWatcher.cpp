#include "../FileWatcher.h"
#include <Cool/File/File.h>
#include <Cool/Log/ToUser.h>

namespace Cool {

FileWatcher::FileWatcher(std::function<void(std::string_view)> on_file_changed, float delay_between_checks)
    : _on_file_changed(on_file_changed), _delay_between_checks(delay_between_checks)
{
}

void FileWatcher::update()
{
    check_and_update_path_validity();
    if (_path_is_valid) {
        // Init static variable (it is static so that we don't need to include chrono in the header file)
        static auto _time_of_last_check = std::chrono::steady_clock::now();
        // Wait for delay between checks
        auto                         now          = std::chrono::steady_clock::now();
        std::chrono::duration<float> elapsed_time = now - _time_of_last_check;
        if (elapsed_time.count() > _delay_between_checks) {
            _time_of_last_check = now;
            // Check file exists
            if (File::exists(_path.string())) {
                // Check file was updated since last check
                auto last_change = std::filesystem::last_write_time(_path);
                if (last_change != _time_of_last_change) {
                    // Apply
                    _time_of_last_change = last_change;
                    _on_file_changed(_path.string());
                }
            }
        }
    }
}

void FileWatcher::set_path(std::string_view path)
{
    _path          = path;
    _path_is_valid = false; // Forces check_and_update_path_validity() to trigger an update (if the new path is valid)
    check_and_update_path_validity();
    if (!_path_is_valid) {
        Log::ToUser::error("FileWatcher::set_path", "Invalid file path : \"{}\"", path);
    }
}

void FileWatcher::check_and_update_path_validity()
{
    bool was_valid = _path_is_valid;
    _path_is_valid = File::exists(_path.string()) && !std::filesystem::is_directory(_path);
    if (!was_valid && _path_is_valid) {
        _time_of_last_change = std::filesystem::last_write_time(_path);
        _on_file_changed(_path.string());
    }
}

} // namespace Cool