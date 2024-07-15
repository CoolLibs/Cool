#include "default_textures.h"

namespace Cool {

auto dummy_texture() -> Texture const&
{
    static auto instance = texture_from_pixels(img::Size{1, 1}, wgpu::TextureFormat::RGBA8Unorm, AlphaSpace::Any, std::array<uint8_t, 4>{255, 0, 255, 255});
    return instance;
}

auto transparent_texture() -> Texture const&
{
    static auto instance = texture_from_pixels(img::Size{1, 1}, wgpu::TextureFormat::RGBA8Unorm, AlphaSpace::Any, std::array<uint8_t, 4>{0, 0, 0, 0});
    return instance;
}

} // namespace Cool
