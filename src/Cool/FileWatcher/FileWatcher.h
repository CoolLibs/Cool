#pragma once

#include <filesystem>

namespace Cool {

/// <summary>
/// Regularily looks for updates of a file and triggers a callback when the file changes. You must call the update() method on every frame.
/// </summary>
class FileWatcher {
public:
	/// <summary>
	/// Regularily looks for updates of a file and triggers the callback when the file changes. The callback is also called at the creation of the FileWatcher.
	/// </summary>
	/// <param name="path">Path of the file to watch</param>
	/// <param name="onFileChanged">Callback function called whenever the file changes. Receives the path as parameter, just for convenience.</param>
	/// <param name="delayBetweenChecks"> In milliseconds : time between two checks. The smaller the delay the bigger the performance cost.</param>
	FileWatcher(const char* path, std::function<void(const char*)> onFileChanged, Uint32 delayBetweenChecksInMS = 500);
	~FileWatcher() = default;
	/// <summary>
	/// Must be called on every frame. Checks if the delay is elapsed, if so checks if the file has changed, and if so calls the callback.
	/// </summary>
	void update();

private:
	std::filesystem::path m_path;
	std::function<void(const char*)> m_onFileChanged;
	Uint32 m_delayBetweenChecksInMS;
	Uint32 m_timeOfLastCheckInMS;
	std::filesystem::file_time_type m_timeOfLastChange;
};

} // namespace Cool