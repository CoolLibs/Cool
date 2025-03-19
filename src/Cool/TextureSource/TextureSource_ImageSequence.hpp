#pragma once
#include "Cool/Gpu/Texture.h"
#include "Cool/Path/Path.h"
#include "Cool/Video/ImageSequence.hpp"

namespace Cool {

struct TextureSource_ImageSequence {
    auto               imgui_widget() -> bool;
    [[nodiscard]] auto get_texture() const -> Texture const*;
    [[nodiscard]] auto get_error() const -> std::optional<std::string>;

    friend auto operator==(TextureSource_ImageSequence const& a, TextureSource_ImageSequence const& b) -> bool = default;

private:
    ImageSequence image_sequence;

private:
    // Serialization
    friend class ser20::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            // ser20::make_nvp("Path", absolute_path)
        );
    }
};

} // namespace Cool