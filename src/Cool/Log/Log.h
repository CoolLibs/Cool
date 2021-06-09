#pragma once

#include <spdlog/spdlog.h>

namespace Cool::Log {
	/**
	 * @brief Sets the log messages pattern.
	 * 
	 * @param spdlog_pattern The pattern to use. See https://github.com/gabime/spdlog/wiki/3.-Custom-formatting#pattern-flags
	 */
	inline void initialize(std::string_view spdlog_pattern = "%^[%e] %n: %v%$") {
		spdlog::set_pattern(std::string(spdlog_pattern));
	}
	
	template<typename ...Args> inline void info(Args&& ...args) {
#ifndef NDEBUG
		spdlog::info(std::forward<Args>(args)...);
#endif
	}
	template<typename ...Args> inline void warn(Args&& ...args) {
#ifndef NDEBUG
		spdlog::warn(std::forward<Args>(args)...);
#endif
	}
	template<typename ...Args> inline void error(Args&& ...args) {
#ifndef NDEBUG
		spdlog::error(std::forward<Args>(args)...);
		assert(false);
#endif
	}
	template<typename ...Args> inline void error_without_breakpoint(Args&& ...args) {
#ifndef NDEBUG
		spdlog::error(std::forward<Args>(args)...);
#endif
	}

class ToUser {
public:
	template<typename ...Args> static inline void info(Args&& ...args) {
		Message(fmt::format(std::forward<Args>(args)...));
	}
	template<typename ...Args> static inline void warn(Args&& ...args) {
		Message(fmt::format(std::forward<Args>(args)...));
	}
	template<typename ...Args> static inline void error(Args&& ...args) {
		Message(fmt::format(std::forward<Args>(args)...));
	}
	static void imgui_console_window();
	static void imgui_toggle_console();

private:
	static void Message(std::string_view message);

private:
	static std::string _message;
	static bool _open;
	static int _messages_count;
	static bool _scroll_to_bottom;
};

} // namespace Cool::Log