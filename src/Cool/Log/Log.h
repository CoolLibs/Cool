#pragma once

#include <spdlog/spdlog.h>

namespace Cool::Log {
/**
 * @brief Sets the log messages pattern.
 * 
 * @param spdlog_pattern The pattern to use. See https://github.com/gabime/spdlog/wiki/3.-Custom-formatting#pattern-flags
 */
inline void initialize(std::string_view spdlog_pattern = "%^[%e] %n: %v%$")
{
    spdlog::set_pattern(std::string(spdlog_pattern));
}

/**
 * @brief Displays a green message to the console in debug mode (in release mode this function does nothing).
 * 
 * @tparam Args 
 * @param args Either one value of any type, or a string followed by as many values as there is {} in the string. Each {} is replaced by one of the arguments passed after the string.
 */
template<typename... Args>
inline void info(Args&&... args)
{
#if defined(DEBUG)
    spdlog::info(std::forward<Args>(args)...);
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
#if defined(DEBUG)
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
#if defined(DEBUG)
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
#if defined(DEBUG)
    spdlog::error(std::forward<Args>(args)...);
#endif
}

} // namespace Cool::Log