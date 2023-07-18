#pragma once
#include <optional>
#include "Cool/Gpu/Texture.h"
#include "TextureSource_FromFile.h"
#include "TextureSource_FromWebcam.h"

namespace Cool {

static auto gen_dummy_texture() -> Texture
{
    return Texture{img::Size{1, 1}, 3, std::array<uint8_t, 3>{255, 0, 255}.data()};
}

using TextureSource = std::variant<
    TextureSource_FromFile,
    TextureSource_FromWebcam>;

inline auto get_texture(TextureSource const& source) -> Texture const&
{
    return std::visit(
        [](auto const& source) -> Texture const& {
            static auto const dummy_tex = gen_dummy_texture();
            if (source.get_texture())
                return source.get_texture().value();

            return dummy_tex;
        },
        source
    );
}

inline auto imgui_widget(TextureSource& source) -> bool
{
    return std::visit(
        [](auto&& source) {
            return source.imgui_widget();
        },
        source
    );
}

inline auto get_error(const TextureSource& source) -> std::optional<std::string>
{
    return std::visit(
        [](auto const& source) -> std::optional<std::string> {
            return source.get_error();
        },
        source
    );
}

} // namespace Cool