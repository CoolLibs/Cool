#include "TextureSource_FromWebcam.h"
#include "TextureLibrary_FromWebcam.h"

namespace Cool {

auto TextureSource_FromWebcam::imgui_widget() -> bool
{
    bool b = false;
    b |= TextureLibrary_FromWebcam::instance().imgui_widget_webcam_name(webcam_name);
    return b;
}

[[nodiscard]] auto TextureSource_FromWebcam::get_texture() const -> Texture const*
{
    return TextureLibrary_FromWebcam::instance().get_webcam_texture(webcam_name);
}

auto TextureSource_FromWebcam::get_error() const -> std::optional<std::string>
{
    return Cool::TextureLibrary_FromWebcam::instance().error_from(webcam_name);
}

} // namespace Cool