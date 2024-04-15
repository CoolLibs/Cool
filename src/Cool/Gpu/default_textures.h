#pragma once
#include "Texture.h"

namespace Cool {

inline auto dummy_texture() -> Texture const&
{
    static auto instance = Texture{img::Size{1, 1}, 3, std::array<uint8_t, 3>{255, 0, 255}.data()};
    return instance;
}
inline auto transparent_texture() -> Texture const&
{
    static auto instance = Texture{img::Size{1, 1}, 4, std::array<uint8_t, 4>{0, 0, 0, 0}.data()};
    return instance;
}

} // namespace Cool