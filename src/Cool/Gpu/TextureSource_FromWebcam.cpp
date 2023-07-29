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
    b |= TextureLibrary_FromWebcam::instance().imgui_widget_webcam_name(webcam_name);

    return b;
}

[[nodiscard]] auto TextureSource_FromWebcam::get_texture() const -> std::optional<Texture> const&
{
    // if (!TextureLibrary_FromWebcam::instance().get_webcam_texture(webcam_name).has_value())
    // {
    //     Cool::Log::ToUser::console()
    //         .send(
    //             Message{
    //                 .category = "Nodes",
    //                 .message  = fmt::format("Cannot open Webcam {}", webcam_name),
    //                 .severity = MessageSeverity::Warning,
    //             }
    //         );
    // }
    return TextureLibrary_FromWebcam::instance().get_webcam_texture(webcam_name);
}

auto TextureSource_FromWebcam::get_error() const -> std::optional<std::string>
{
    auto const err = Cool::TextureLibrary_FromWebcam::instance().error_from(webcam_name);

    if (err)
    {
        Cool::Log::ToUser::console()
            .send(
                Message{
                    .category = "Nodes",
                    .message  = fmt::format("Failed to read node from Camera {}:\n", webcam_name),
                    .severity = MessageSeverity::Warning,
                }
            );
        return err; // no need
    }
    return std::nullopt;
}

} // namespace Cool