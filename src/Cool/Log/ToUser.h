#pragma once

#include "internal/Message.h"

namespace Cool::Log {

/**
 * @brief If you want your end user to see a message, use these functions. The messages pop-up in a "console" ImGui window.
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
    template<typename... Args>
    static inline void info(std::string_view category, Args&&... args)
    {
        add_message(
            {Message::Severity::Info,
             std::string{category},
             fmt::format(std::forward<Args>(args)...)});
    }

    /**
	 * @brief Displays a message in the ImGui console.
	 * 
	 * @tparam Args 
	 * @param args 
	 */
    template<typename... Args>
    static inline void warn(std::string_view category, Args&&... args)
    {
        add_message(
            {Message::Severity::Warn,
             std::string{category},
             fmt::format(std::forward<Args>(args)...)});
    }

    /**
	 * @brief Displays a message in the ImGui console.
	 * 
	 * @tparam Args 
	 * @param args Either one value of any type, or a string followed by as many values as there is {} in the string. Each {} is replaced by one of the arguments passed after the string.
	 */
    template<typename... Args>
    static inline void error(std::string_view category, Args&&... args)
    {
        // add_message(
        //     {Message::Severity::Error,
        //      std::string{category},
        //      fmt::format(std::forward<Args>(args)...)});
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
    static void add_message(Message message);

private:
    static std::vector<Message> _messages;
    static bool                 _is_open;
    static bool                 _scroll_to_bottom;
};
} // namespace Cool::Log