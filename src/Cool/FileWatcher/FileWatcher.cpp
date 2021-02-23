#include "FileWatcher.h"

#include <Cool/File/File.h>
#include <chrono>

namespace Cool {

FileWatcher::FileWatcher(std::string_view path, std::function<void(const char*)> onFileChanged, float delayBetweenChecks)
	: m_onFileChanged(onFileChanged), m_delayBetweenChecks(delayBetweenChecks),
	  m_timeOfLastChange(std::filesystem::last_write_time(path))
{
	setPath(path);
}

void FileWatcher::update() {
	// Init static variable (it is static so that we don't need to include chrono in the header file)
	static auto m_timeOfLastCheck = std::chrono::steady_clock::now();
	// Wait for delay between checks
	auto now = std::chrono::steady_clock::now();
	std::chrono::duration<float> elapsedTime = now - m_timeOfLastCheck;
	if (elapsedTime.count() > m_delayBetweenChecks) {
		m_timeOfLastCheck = now;
		// Check file exists
		if (File::Exists(m_path.string().c_str())) {
			// Check file was updated since last check
			auto lastChange = std::filesystem::last_write_time(m_path);
			if (lastChange != m_timeOfLastChange) {
				// Apply
				m_timeOfLastChange = lastChange;
				m_onFileChanged(m_path.string().c_str());
			}
		}
	}
}

void FileWatcher::setPath(std::string_view path) {
	m_path = path;
	m_onFileChanged(m_path.string().c_str());
}

} // namespace Cool