#pragma once
#include "Cool/Gpu/Texture.h"
#include "internal/WebcamRequest.h"

namespace Cool {

class TextureLibrary_FromWebcam {
public:
    [[nodiscard]] static auto instance() -> TextureLibrary_FromWebcam&;

    auto               get_texture(std::string const& name) -> Texture const*;
    [[nodiscard]] auto has_active_webcams() const -> bool;
    [[nodiscard]] auto error_from(std::string const& webcam_name) const -> std::optional<std::string>;

private:
    friend class WebcamsConfigs;
    void invalidate_request(std::string const& webcam_name);

private:
    friend class AppManager;
    void on_frame_begin();
    void on_frame_end();

private:
    TextureLibrary_FromWebcam() = default; // This is a singleton. Get the global instance with `instance()` instead.
    auto get_request(std::string const& webcam_name) -> WebcamRequest*;

private:
    std::vector<WebcamRequest> _requests;
};

} // namespace Cool