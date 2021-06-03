#pragma once

#include <spdlog/spdlog.h>

namespace Cool::Log {
	/// <summary>
	/// </summary>
	/// <param name="spdlogPattern">: Sets the log messages pattern ; see here to configure it : https://github.com/gabime/spdlog/wiki/3.-Custom-formatting#pattern-flags</param>
	inline void Initialize(const char* spdlogPattern = "%^[%e] %n: %v%$") {
		spdlog::set_pattern(spdlogPattern);
	}
	// 
	template<typename ...Args> inline void Info(Args&& ...args) {
#ifndef NDEBUG
		spdlog::info(std::forward<Args>(args)...);
#endif
	}
	template<typename ...Args> inline void Warn(Args&& ...args) {
#ifndef NDEBUG
		spdlog::warn(std::forward<Args>(args)...);
#endif
	}
	template<typename ...Args> inline void Error(Args&& ...args) {
#ifndef NDEBUG
		spdlog::error(std::forward<Args>(args)...);
		assert(false);
#endif
	}
	template<typename ...Args> inline void ErrorWithoutBreakpoint(Args&& ...args) {
#ifndef NDEBUG
		spdlog::error(std::forward<Args>(args)...);
#endif
	}

class ToUser {
public:
	template<typename ...Args> static inline void Info(Args&& ...args) {
		Message(fmt::format(std::forward<Args>(args)...));
	}
	template<typename ...Args> static inline void Warn(Args&& ...args) {
		Message(fmt::format(std::forward<Args>(args)...));
	}
	template<typename ...Args> static inline void Error(Args&& ...args) {
		Message(fmt::format(std::forward<Args>(args)...));
	}
	static void Show_Console();
	static void ImGui_Toggle_Console();

private:
	static void Message(std::string_view message);

private:
	static std::string _message;
	static bool _open;
	static int _messages_count;
	static bool _scroll_to_bottom;
};

} // namespace Cool::Log