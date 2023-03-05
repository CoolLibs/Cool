#include "FileWatcher.h"
#include <Cool/File/File.h>

namespace Cool {

static auto is_valid_file_path(std::filesystem::path const& path) -> bool
{
    return File::exists(path) && !std::filesystem::is_directory(path);
}

static auto time_of_last_change(std::filesystem::path const& path) -> std::optional<std::filesystem::file_time_type>
{
    try
    {
        return std::filesystem::last_write_time(path);
    }
    catch (...)
    {
        return std::nullopt;
    }
}

FileWatcher::FileWatcher(std::filesystem::path const& path, FileWatcher_Callbacks const& callbacks, FileWatcher_Config config)
    : _config{config}
{
    set_path(path, callbacks);
}

void FileWatcher::set_path(std::filesystem::path const& path, FileWatcher_Callbacks const& callbacks)
{
    _path                = path;
    _time_of_last_change = time_of_last_change(path);
    _time_of_last_check  = clock::now();
    call_appropriate_callback(callbacks);
}

void FileWatcher::update(FileWatcher_Callbacks const& callbacks) const
{
    { // Wait for the delay between checks
        auto const now          = clock::now();
        auto const elapsed_time = std::chrono::duration<float>{now - _time_of_last_check};
        if (elapsed_time.count() < _config.delay_between_checks)
            return;
        _time_of_last_check = now;
    }

    { // Check if the file was updated since last check
        auto const last_change = time_of_last_change(_path);
        if (last_change == _time_of_last_change)
            return;
        _time_of_last_change = last_change;
    }

    call_appropriate_callback(callbacks);
}

void FileWatcher::call_appropriate_callback(FileWatcher_Callbacks const& callbacks) const
{
    if (path_is_valid())
        callbacks.on_file_changed(_path);
    else
        callbacks.on_path_invalid(_path);
}

auto FileWatcher::path_is_valid() const -> bool
{
    return _time_of_last_change != std::nullopt;
}

} // namespace Cool