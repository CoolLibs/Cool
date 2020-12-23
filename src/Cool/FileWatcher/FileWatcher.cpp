#include "FileWatcher.h"

#include <Cool/File/File.h>

namespace Cool {

FileWatcher::FileWatcher(const char* path, std::function<void(const char*)> onFileChanged, Uint32 delayBetweenChecksInMS)
	: m_path(path), m_onFileChanged(onFileChanged), m_delayBetweenChecksInMS(delayBetweenChecksInMS),
	  m_timeOfLastChange(std::filesystem::last_write_time(m_path)), m_timeOfLastCheckInMS(SDL_GetTicks())
{
	m_onFileChanged(m_path.string().c_str());
}

void FileWatcher::update() {
	// Wait for delay between checks
	Uint32 timeInMS = SDL_GetTicks();
	if (timeInMS - m_timeOfLastCheckInMS > m_delayBetweenChecksInMS) {
		m_timeOfLastCheckInMS = timeInMS;
		// Check file exists
		if (File::Exists(m_path.string().c_str())) {
			// Check file was updated since last check
			std::filesystem::file_time_type lastChange = std::filesystem::last_write_time(m_path);
			if (lastChange != m_timeOfLastChange) {
				// Apply
				m_timeOfLastChange = lastChange;
				m_onFileChanged(m_path.string().c_str());
			}
		}
	}
}

} // namespace Cool