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
	//
	namespace Release {
		template<typename ...Args> inline void Info(Args&& ...args) {
			spdlog::info(std::forward<Args>(args)...);
		}
		template<typename ...Args> inline void Warn(Args&& ...args) {
			spdlog::warn(std::forward<Args>(args)...);
		}
		template<typename ...Args> inline void Error(Args&& ...args) {
			spdlog::error(std::forward<Args>(args)...);
			assert(false);
		}
	}
}