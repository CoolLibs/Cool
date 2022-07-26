#pragma once

#include <Cool/MessageConsole/MessageConsole.h>
#include <spdlog/spdlog.h>

namespace Cool::Log {

#if DEBUG
inline auto console() -> MessageConsole&
{
    static auto the_console = MessageConsole{"Debug Console"};
    return the_console;
}
#endif

/**
 * @brief Displays a green message to the console in debug mode (in release mode this function does nothing).
 *
 * @tparam Args
 * @param args Either one value of any type, or a string followed by as many values as there is {} in the string. Each {} is replaced by one of the arguments passed after the string.
 */
inline void info(const std::string& category, const std::string& message) // We take string& instead of string_view because Message needs strings anyways
{
#if DEBUG
    console().send(MessageV2{
        .category         = category,
        .detailed_message = message,
        .severity         = MessageSeverity::Info,
    });
#endif
}

/**
 * @brief Displays a yellow message to the console in debug mode (in release mode this function does nothing).
 *
 * @tparam Args
 * @param args Either one value of any type, or a string followed by as many values as there is {} in the string. Each {} is replaced by one of the arguments passed after the string.
 */
template<typename... Args>
inline void warn(Args&&... args)
{
#if DEBUG
    spdlog::warn(std::forward<Args>(args)...);
#endif
}

/**
 * @brief Displays a red message to the console and triggers a breakpoint in debug mode (in release mode this function does nothing).
 *
 * @tparam Args
 * @param args Either one value of any type, or a string followed by as many values as there is {} in the string. Each {} is replaced by one of the arguments passed after the string.
 */
template<typename... Args>
inline void error(Args&&... args)
{
#if DEBUG
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
template<typename... Args>
inline void error_without_breakpoint(Args&&... args)
{
#if DEBUG
    spdlog::error(std::forward<Args>(args)...);
#endif
}

} // namespace Cool::Log