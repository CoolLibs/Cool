#pragma once
#include "TextureSource_FromWebcam.h"
#include <string>
#include "Cool/Gpu/TextureLibrary_FromWebcam.h"
#include "Cool/Gpu/TextureSource_FromWebcam.h"
#include "TextureLibrary_FromWebcam.h"

namespace Cool {

auto TextureSource_FromWebcam::imgui_widget() -> bool
{
    bool b = false;
    b |= TextureLibrary_FromWebcam::instance().imgui_widget_webcam_index(webcam_index);

    return b;
}

[[nodiscard]] auto TextureSource_FromWebcam::get_texture() const -> Texture const&
{
    return TextureLibrary_FromWebcam::instance().get_webcam_texture(webcam_index); // TODO(TD) avoir un shared pointer ???? meh :/
}

} // namespace Cool