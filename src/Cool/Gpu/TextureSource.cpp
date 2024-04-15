#include "TextureSource.h"
#include "default_textures.h"

namespace Cool {

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