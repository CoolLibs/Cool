#include "TextureSource.h"
#include "Cool/ColorSpaces/AlphaSpace.h"
#include "Cool/WebGPU/Texture.h"

namespace Cool {

static auto dummy_texture() -> Texture const&
{
    static auto instance = texture_from_pixels(img::Size{1, 1}, wgpu::TextureFormat::RGBA8Unorm, AlphaSpace::Any, std::array<uint8_t, 4>{255, 0, 255, 255});
    return instance;
}

auto get_texture(TextureSource const& source) -> Texture const&
{
    return std::visit(
        [](auto const& source) -> Texture const& {
            auto const* texture = source.get_texture();
            if (!texture)
                return dummy_texture();

            return *texture;
        },
        source
    );
}

auto imgui_widget(TextureSource& source) -> bool
{
    return std::visit([](auto& source) { return source.imgui_widget(); }, source);
}

auto get_error(const TextureSource& source) -> std::optional<std::string>
{
    return std::visit([](auto const& source) { return source.get_error(); }, source);
}

} // namespace Cool