#pragma once

namespace Cool::UserInput {

/**
 * @brief Checks if a glfw key-code corresponds to a given character in the current keyboard layout
 *
 * @param character A const char* containing the single character that you want to check against
 * @param key The key code that you want to check
 */
bool matches_char(const char* character, int key);

} // namespace Cool::UserInput