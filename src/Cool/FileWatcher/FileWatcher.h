#pragma once

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
	FileWatcher(std::function<void(std::string_view)> on_file_changed, float delay_between_checks = 0.5f);

	/**
	 * @brief Must be called on every frame. Checks if the delay is elapsed, if so checks if the file has changed, and if so calls the callback
	 * 
	 */
	void update();

	/**
	 * @brief 
	 * 
	 * @return The path to the file we are currently watching
	 */
	inline const std::filesystem::path path() const { return _path; }
	
	/**
	 * @brief Sets the path to watch, and calls the on_file_changed callback if the path is valid
	 * 
	 * @param path Path of the file to watch
	 */
	void set_path(std::string_view path);

	/**
	 * @brief 
	 * 
	 * @return true iff we are currently watching a valid file path (meaning the file exists and we are allowed to read it)
	 */
	inline bool path_is_valid() const { return _path_is_valid; }

private:
	void check_and_update_path_validity();

private:
	std::filesystem::path _path = "";
	bool _path_is_valid = false;
	std::function<void(std::string_view)> _on_file_changed;
	float _delay_between_checks;
	std::filesystem::file_time_type _time_of_last_change;
};

} // namespace Cool