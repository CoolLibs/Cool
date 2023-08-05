#pragma once
#include <compare>
#include "Cool/Gpu/Texture.h"
#include "Cool/Path/Path.h"
namespace Cool {

struct TextureSource_FromFile {
    std::filesystem::path absolute_path{Cool::Path::default_texture()};

    auto               imgui_widget() -> bool;
    [[nodiscard]] auto get_texture() const -> Texture const*;
    auto               get_error() const -> std::optional<std::string>;

    friend auto operator==(TextureSource_FromFile const&, TextureSource_FromFile const&) -> bool = default;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Path", absolute_path)
        );
    }
};
} // namespace Cool