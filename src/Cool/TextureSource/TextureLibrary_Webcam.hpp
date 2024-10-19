#pragma once
#include "Cool/Gpu/Texture.h"
#include "wcam/wcam.hpp"

namespace Cool {

class TextureLibrary_Webcam {
public:
    [[nodiscard]] static auto instance() -> TextureLibrary_Webcam&;

    [[nodiscard]] auto get_texture(wcam::DeviceId const&) -> Texture const*;
    [[nodiscard]] auto get_error(wcam::DeviceId const&) const -> std::optional<std::string>;

    void shut_down();

private:
    friend class AppManager;
    void on_frame_end();

private:
    TextureLibrary_Webcam() = default; // This is a singleton. Get the global instance with `instance()` instead.

private:
    struct WebcamData {
        wcam::SharedWebcam webcam;
        wcam::MaybeImage   maybe_image{};
        bool               has_been_requested_this_frame{false};
    };
    std::vector<WebcamData> _webcams{};
};

} // namespace Cool