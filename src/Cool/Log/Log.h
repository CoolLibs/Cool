#pragma once

#include <Cool/MessageConsole/MessageConsole.h>
#include <spdlog/spdlog.h>

namespace Cool::Log {

/// Displays a green message in the debug console.
/// Does nothing in release mode.
void info(const std::string& category, const std::string& message);

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

#if DEBUG
auto console() -> MessageConsole&;
#endif

} // namespace Cool::Log