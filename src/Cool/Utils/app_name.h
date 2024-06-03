#pragma once

namespace Cool {

inline auto app_name() -> std::string&
{
    static auto instance = std::string{};
    return instance;
}

} // namespace Cool