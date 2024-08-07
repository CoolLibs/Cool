#pragma once
#include <optional>
#include <string>
#include "Cool/Gpu/Texture.h"
#include "Cool/Webcam/WebcamsInfos.h"

namespace Cool {

struct TextureSource_Webcam {
    std::string webcam_name{Cool::WebcamsInfos::instance().default_webcam_name()}; // NB: most of the time a Source is created too early for this to have any effect (the thread that finds the webcam's names hasn't had time to do its job yet). This is why we have a HACK in TextureLibrary_FromWebcam::imgui_widget_webcam_name to still init with a proper webcam name.

    auto               imgui_widget() -> bool;
    [[nodiscard]] auto get_texture() const -> Texture const*;
    [[nodiscard]] auto get_error() const -> std::optional<std::string>;

    friend auto operator==(TextureSource_Webcam const&, TextureSource_Webcam const&) -> bool = default;

private:
    // Serialization
    friend class ser20::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            ser20::make_nvp("Webcam Name", webcam_name)
        );
    }
};

} // namespace Cool