#pragma once

#include <filesystem>

namespace Cool {

/// <summary>
/// Regularily looks for updates of a file and triggers a callback when the file changes. You must call the update() method on every frame.
/// </summary>
class FileWatcher {
public:
	/// <summary>
	/// Regularily looks for updates of a file and triggers the callback when the file changes. The callback is also called when the path is set.
	/// </summary>
	/// <param name="onFileChanged">Callback function called whenever the file changes. Receives the path as parameter, just for convenience.</param>
	/// <param name="delayBetweenChecks"> In seconds : time between two checks. The smaller the delay the bigger the performance cost.</param>
	FileWatcher(std::function<void(const char*)> onFileChanged, float delayBetweenChecks = 0.5f);
	~FileWatcher() = default;

	/// <summary>
	/// Must be called on every frame. Checks if the delay is elapsed, if so checks if the file has changed, and if so calls the callback.
	/// </summary>
	void update();

	/// <summary>
	/// Returns the path to the file we are currently watching
	/// </summary>
	inline const std::filesystem::path& path() const { return m_path; }

	/// <summary>
	/// Sets the path to watch, and calls the onFileChanged callback if the path is valid.
	/// </summary>
	/// <param name="path">Path of the file to watch</param>
	void setPath(std::string_view path);

	/// <summary>
	/// 
	/// </summary>
	/// <returns>true iff we are currently watching a valid file path (meaning the file exists and we are allowed to read it)</returns>
	inline bool pathIsValid() const { return m_bPathIsValid; }

private:
	void checkAndUpdatePathValidity();

private:
	std::filesystem::path m_path = "";
	bool m_bPathIsValid = false;
	std::function<void(const char*)> m_onFileChanged;
	float m_delayBetweenChecks;
	std::filesystem::file_time_type m_timeOfLastChange;
};

} // namespace Cool