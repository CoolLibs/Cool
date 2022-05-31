#pragma once

#include <Cool/Log/ToUser.h>
#include <filesystem>

namespace Cool {

struct FileWatcher_Config {
    /// In seconds. The smaller the delay the bigger the performance cost.
    float delay_between_checks = 0.5f;
};

struct FileWatcher_Callbacks {
    /// Callback called whenever the file changes. Receives the currently watched path as a parameter.
    std::function<void(std::string_view)> on_file_changed;
    /// Callback called whenever the path becomes invalid. Receives the currently watched path as a parameter.
    std::function<void(std::string_view)> on_path_invalid = [](auto path) { Log::ToUser::error("File Watcher", "Invalid file path : \"{}\"", path); };
};

/**
 * @brief Regularily looks for updates of a file and triggers a callback when the file changes. You must call the update() method on every frame.
 */
class FileWatcher {
public:
    explicit FileWatcher(std::filesystem::path path = {}, FileWatcher_Config config = {});

    /**
	 * @brief Must be called on every frame. Checks if the delay is elapsed, if so checks if the file has changed, and if so calls the appropriate callback.
	 */
    void update(FileWatcher_Callbacks) const;

    /**
	 * @brief The path we are currently watching
	 */
    auto path() const -> std::filesystem::path { return _path; }

    /**
     * @brief Whether the path we are watching is valid
     */
    auto path_is_valid() const -> bool { return std::holds_alternative<Valid>(_path_validity); };

    /**
	 * @brief Sets the path to watch.
     * The appropriate callback will then be called in the next update(), depending on the path validity (either on_file_changed() or on_path_invalid())
	 */
    void set_path(std::string_view path);

private:
    void on_file_changed(FileWatcher_Callbacks) const;
    void on_path_invalid(FileWatcher_Callbacks) const;

private:
    struct Valid {
    };
    struct Invalid {
    };
    struct Unknown {
    };
    using PathValidity = std::variant<Valid, Invalid, Unknown>;
    void update_validity(Valid, bool is_valid, FileWatcher_Callbacks) const;
    void update_validity(Invalid, bool is_valid, FileWatcher_Callbacks) const;
    void update_validity(Unknown, bool is_valid, FileWatcher_Callbacks) const;

private:
    std::filesystem::path                   _path;
    mutable PathValidity                    _path_validity = Unknown{};
    mutable std::filesystem::file_time_type _time_of_last_change;
    FileWatcher_Config                      _config;
};

} // namespace Cool