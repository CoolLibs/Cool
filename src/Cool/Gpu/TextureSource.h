#pragma once
#include <optional>
#include "Cool/Gpu/Texture.h"
#include "TextureSource_FromFile.h"
#include "TextureSource_FromWebcam.h"

namespace Cool {

using TextureSource = std::variant<
    TextureSource_FromFile,
    TextureSource_FromWebcam>;

inline auto get_texture(TextureSource const& source) -> Texture const&
{
    return std::visit(
        [](auto const& source) -> Texture const& {
            return source.get_texture();
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
            source.get_error();
        },
        source
    );
}

} // namespace Cool