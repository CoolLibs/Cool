#pragma once

#include <spdlog/spdlog.h>
#include "internal/Message.h"

namespace Cool::Log {
	/**
	 * @brief Sets the log messages pattern.
	 * 
	 * @param spdlog_pattern The pattern to use. See https://github.com/gabime/spdlog/wiki/3.-Custom-formatting#pattern-flags
	 */
	inline void initialize(std::string_view spdlog_pattern = "%^[%e] %n: %v%$") {
		spdlog::set_pattern(std::string(spdlog_pattern));
	}
	
	/**
	 * @brief Displays a green message to the console in debug mode (in release mode this function does nothing).
	 * 
	 * @tparam Args 
	 * @param args Either one value of any type, or a string followed by as many values as there is {} in the string. Each {} is replaced by one of the arguments passed after the string.
	 */
	template<typename ...Args> inline void info(Args&& ...args) {
#ifdef DEBUG
		spdlog::info(std::forward<Args>(args)...);
#endif
	}

	/**
	 * @brief Displays a yellow message to the console in debug mode (in release mode this function does nothing).
	 * 
	 * @tparam Args 
	 * @param args Either one value of any type, or a string followed by as many values as there is {} in the string. Each {} is replaced by one of the arguments passed after the string.
	 */
	template<typename ...Args> inline void warn(Args&& ...args) {
#ifdef DEBUG
		spdlog::warn(std::forward<Args>(args)...);
#endif
	}

	/**
	 * @brief Displays a red message to the console and triggers a breakpoint in debug mode (in release mode this function does nothing).
	 * 
	 * @tparam Args 
	 * @param args Either one value of any type, or a string followed by as many values as there is {} in the string. Each {} is replaced by one of the arguments passed after the string.
	 */
	template<typename ...Args> inline void error(Args&& ...args) {
#ifdef DEBUG
		spdlog::error(std::forward<Args>(args)...);
		assert(false);
#endif
	}

	/**
	 * @brief Displays a red message to the console in debug mode (in release mode this function does nothing).
	 * 
	 * @tparam Args 
	 * @param args Either one value of any type, or a string followed by as many values as there is {} in the string. Each {} is replaced by one of the arguments passed after the string.
	 */
	template<typename ...Args> inline void error_without_breakpoint(Args&& ...args) {
#ifdef DEBUG
		spdlog::error(std::forward<Args>(args)...);
#endif
	}

/**
 * @brief If you want your end user to see a message, use these functions. The messages pop-up in an ImGui window called the console.
 * 
 */
class ToUser {
public:
	/**
	 * @brief Displays a message in the ImGui console.
	 * 
	 * @tparam Args 
	 * @param args Either one value of any type, or a string followed by as many values as there is {} in the string. Each {} is replaced by one of the arguments passed after the string.
	 */
	template<typename ...Args> static inline void info(std::string_view category, Args&& ...args) {
		PushMessage({
			Message::Severity::Info,
			std::string{category},
			fmt::format(std::forward<Args>(args)...)
		});
	}

	/**
	 * @brief Displays a message in the ImGui console.
	 * 
	 * @tparam Args 
	 * @param args 
	 */
	template<typename ...Args> static inline void warn(std::string_view category, Args&& ...args) {
		PushMessage({
			Message::Severity::Warn,
			std::string{category},
			fmt::format(std::forward<Args>(args)...)
		});
	}

	/**
	 * @brief Displays a message in the ImGui console.
	 * 
	 * @tparam Args 
	 * @param args Either one value of any type, or a string followed by as many values as there is {} in the string. Each {} is replaced by one of the arguments passed after the string.
	 */
	template<typename ...Args> static inline void error(std::string_view category, Args&& ...args) {
		PushMessage({
			Message::Severity::Error,
			std::string{category},
			fmt::format(std::forward<Args>(args)...)
		});
	}

	/**
	 * @brief If the console is open, shows the console with all the messages.
	 * 
	 */
	static void imgui_console_window();

	/**
	 * @brief Shows a checkbox to open or close the console.
	 * 
	 */
	static void imgui_toggle_console();

private:
	static void PushMessage(Message message);

private:
	static std::vector<Message> _messages;
	static bool _open;
	static int _messages_count;
	static bool _scroll_to_bottom;
};

} // namespace Cool::Log