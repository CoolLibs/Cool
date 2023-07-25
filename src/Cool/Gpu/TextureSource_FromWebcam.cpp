#include "TextureSource_FromWebcam.h"
#include <optional>
#include <string>
#include "Cool/Gpu/TextureLibrary_FromWebcam.h"
#include "Cool/Gpu/TextureSource_FromWebcam.h"
#include "Cool/Log/MessageConsole.h"
#include "Cool/Log/ToUser.h"
#include "TextureLibrary_FromWebcam.h"

namespace Cool {

auto TextureSource_FromWebcam::imgui_widget() -> bool
{
    bool b = false;
    b |= TextureLibrary_FromWebcam::instance().imgui_widget_webcam_index(webcam_index);

    return b;
}

[[nodiscard]] auto TextureSource_FromWebcam::get_texture() const -> std::optional<Texture> const&
{
    return TextureLibrary_FromWebcam::instance().get_webcam_texture(webcam_index);
}

auto TextureSource_FromWebcam::get_error() const -> std::optional<std::string>
{
    auto const err = Cool::TextureLibrary_FromWebcam::instance().error_from(webcam_index);

    if (err)
    {
        Cool::Log::ToUser::console()
            .send(
                Message{
                    .category = "Nodes",
                    .message  = fmt::format("Failed to read node from Camera {}:\n", webcam_index),
                    .severity = MessageSeverity::Warning,
                }
            );
        return err; // no need
    }
    return std::nullopt;
}

} // namespace Cool