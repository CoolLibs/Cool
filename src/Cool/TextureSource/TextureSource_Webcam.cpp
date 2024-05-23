#include "TextureSource_Webcam.h"
#include "Cool/Webcam/TextureLibrary_FromWebcam.h"
#include "Cool/Webcam/WebcamsInfos.h"

namespace Cool {

auto TextureSource_Webcam::imgui_widget() -> bool
{
    return WebcamsInfos::instance().imgui_widget_webcam_name(webcam_name);
}

[[nodiscard]] auto TextureSource_Webcam::get_texture() const -> Texture const*
{
    return TextureLibrary_FromWebcam::instance().get_texture(webcam_name);
}

auto TextureSource_Webcam::get_error() const -> std::optional<std::string>
{
    return Cool::TextureLibrary_FromWebcam::instance().error_from(webcam_name);
}

} // namespace Cool