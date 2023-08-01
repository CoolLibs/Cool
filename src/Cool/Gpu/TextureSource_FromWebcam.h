#pragma once
#include <compare>
#include <optional>
#include <string>
#include "Cool/Gpu/Texture.h"
#include "Cool/Gpu/TextureLibrary_FromWebcam.h"
#include "Cool/Path/Path.h"
namespace Cool {

struct TextureSource_FromWebcam {
    std::string webcam_name{Cool::TextureLibrary_FromWebcam::instance().get_default_webcam_name()};

    auto               imgui_widget() -> bool;
    [[nodiscard]] auto get_texture() const -> std::optional<Texture> const&;
    auto               get_error() const -> std::optional<std::string>;

    friend auto operator==(TextureSource_FromWebcam const&, TextureSource_FromWebcam const&) -> bool = default;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Webcam Name", webcam_name)
        );
    }
};
} // namespace Cool