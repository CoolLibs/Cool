#pragma once

namespace Cool {

/**
 * @brief Assumes that version is encoded like so : 4.3 <-> 430
 * This would return the 4 in that example
 * @param version
 * @return int
 */
inline constexpr int major_version(int version)
{
    return version / 100;
}

/**
 * @brief Assumes that version is encoded like so : 4.3 <-> 430
 * This would return the 3 in that example
 *
 * @param version
 * @return int
 */
inline constexpr int minor_version(int version)
{
    return (version - version / 100 * 100) / 10;
}

} // namespace Cool