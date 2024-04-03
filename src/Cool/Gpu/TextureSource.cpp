#include "TextureSource.h"

namespace Cool {

static auto dummy_texture() -> Texture const&
{
    static auto instance = Texture{/* img::Size{1, 1}, 3, std::array<uint8_t, 3>{255, 0, 255}.data() */}; // TODO(WebGPU)
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