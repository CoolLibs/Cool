#pragma once
#include <optional>
#include <string>
#include "Cool/Gpu/Texture.h"
#include "Cool/Webcam/WebcamsInfos.h"

namespace Cool {

struct TextureSource_FromWebcam {
    std::string webcam_name{Cool::WebcamsInfos::instance().get_default_webcam_name()}; // NB: most of the time a Source is created too early for this to have any effect (the thread that finds the webcam's names hasn't had time to do its job yet). This is why we have a HACK in TextureLibrary_FromWebcam::imgui_widget_webcam_name to still init with a proper webcam name.

    auto               imgui_widget() -> bool;
    [[nodiscard]] auto get_texture() const -> Texture const*;
    [[nodiscard]] auto get_error() const -> std::optional<std::string>;

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