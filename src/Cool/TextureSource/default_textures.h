#pragma once
#include "Cool/Gpu/Texture.h"

namespace Cool {

inline auto dummy_image() -> img::Image const&
{
    static auto instance = img::Image{img::Size{1, 1}, img::PixelFormat::RGB, img::FirstRowIs::Bottom, std::array<uint8_t, 3>{255, 0, 255}.data()};
    return instance;
}
inline auto dummy_texture() -> Texture const&
{
    static auto instance = Texture{dummy_image()};
    return instance;
}
inline auto transparent_texture() -> Texture const&
{
    static auto instance = Texture{img::Size{1, 1}, 4, std::array<uint8_t, 4>{0, 0, 0, 0}.data()};
    return instance;
}

} // namespace Cool