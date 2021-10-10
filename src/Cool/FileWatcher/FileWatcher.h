#pragma once

#include <Cool/Log/ToUser.h>
#include <filesystem>

namespace Cool {

/**
 * @brief Regularily looks for updates of a file and triggers a callback when the file changes. You must call the update() method on every frame
 * 
 */
class FileWatcher {
public:
    /**
	 * @brief Regularily looks for updates of a file and triggers the callback when the file changes. The callback is also called when the path is set
	 * 
	 * @param on_file_changed Callback function called whenever the file changes. Receives the path as parameter, just for convenience
	 * @param In seconds : time between two checks. The smaller the delay the bigger the performance cost 
	 */
    explicit FileWatcher(
        std::filesystem::path                 path,
        std::function<void(std::string_view)> on_file_changed,
        std::function<void(std::string_view)> on_path_invalid      = [](auto path) { Log::ToUser::error("FileWatcher", "Invalid file path : \"{}\"", path); },
        float                                 delay_between_checks = 0.5f);

    /**
	 * @brief Must be called on every frame. Checks if the delay is elapsed, if so checks if the file has changed, and if so calls the callback
	 */
    void update() const;

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
    void on_file_changed() const;
    void on_path_invalid() const;

private:
    struct Valid {
    };
    struct Invalid {
    };
    struct Unknown {
    };
    using PathValidity = std::variant<Valid, Invalid, Unknown>;
    void update_validity(Valid, bool is_valid) const;
    void update_validity(Invalid, bool is_valid) const;
    void update_validity(Unknown, bool is_valid) const;

private:
    std::filesystem::path                   _path;
    mutable PathValidity                    _path_validity = Unknown{};
    mutable std::filesystem::file_time_type _time_of_last_change;
    std::function<void(std::string_view)>   _on_file_changed;
    std::function<void(std::string_view)>   _on_path_invalid;
    float                                   _delay_between_checks;
};

} // namespace Cool