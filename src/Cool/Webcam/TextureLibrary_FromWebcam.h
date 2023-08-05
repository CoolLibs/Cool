#pragma once
#include "Cool/Gpu/Texture.h"
#include "internal/WebcamRequest.h"

namespace Cool {

class TextureLibrary_FromWebcam {
public:
    [[nodiscard]] static auto instance() -> TextureLibrary_FromWebcam&;

    auto               get_webcam_texture(std::string name) -> Texture const*;
    [[nodiscard]] auto has_active_webcam() const -> bool;
    [[nodiscard]] auto error_from(std::string webcam_name) const -> std::optional<std::string>;
    void               invalidate_request(std::string const& request_name);

private:
    friend class AppManager;
    void on_frame_begin(); // TODO(TD)(à test) remet tous les is_dirty à true
    void on_frame_end();   // TODO(TD)(à test) supprime toutes les texture qui sont dirty (car elles n'ont pas été utilisées à cette frame)

private:
    TextureLibrary_FromWebcam() = default; // This is a singleton. Get the global instance with `instance()` instead.
    auto get_request(std::string const& webcam_name) -> WebcamRequest*;

private:
    std::vector<WebcamRequest> _requests;
};

} // namespace Cool