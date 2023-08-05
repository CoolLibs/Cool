#include "TextureLibrary_FromWebcam.h"
#include <webcam_info/webcam_info.hpp>
#include "Cool/Gpu/Texture.h"
#include "WebcamsConfigs.h"
#include "WebcamsInfos.h"

namespace Cool {

auto TextureLibrary_FromWebcam::instance() -> TextureLibrary_FromWebcam&
{
    static auto inst = TextureLibrary_FromWebcam{};
    return inst;
}

auto TextureLibrary_FromWebcam::get_request(std::string const& webcam_name) -> WebcamRequest*
{
    for (auto& request : _requests)
    {
        if (request.webcam_name == webcam_name)
            return &request;
    }
    return nullptr;
}

auto TextureLibrary_FromWebcam::get_texture(std::string const& webcam_name) -> Texture const*
{
    auto*      request      = get_request(webcam_name);
    auto const webcam_index = WebcamsInfos::instance().index(webcam_name);

    if (request == nullptr)
    {
        _requests.emplace_back(webcam_index, webcam_name);
        request = &_requests.back();
    }

    request->has_been_requested_this_frame = true;

    if (!webcam_index.has_value())
    {
        // Cool::Log::ToUser::console()
        //     .send(
        //         request->_iderror_cannot_find_webcam,
        //         Message{
        //             .category = "Webcam",
        //             .message  = fmt::format("cannot find webcam {}", name),
        //             .severity = MessageSeverity::Error,
        //         }
        //     );
        request->capture.reset();
        return nullptr;
    }

    if (!request->capture || request->capture->webcam_index() != *webcam_index)
    {
        request->capture = std::make_unique<WebcamCapture>(
            *webcam_index,
            WebcamsConfigs::instance().selected_resolution(webcam_name)
        );
    }

    if (request->capture->has_stopped())
    {
        // Cool::Log::ToUser::console()
        //     .send(
        //         request->_iderror_cannot_find_webcam,
        //         Message{
        //             .category = "Webcam",
        //             .message  = fmt::format("Failed to capture \"{}\". It might be because it is used in another software", request->name),
        //             .severity = MessageSeverity::Error,
        //         }
        //     );
        request->capture.reset();
        return nullptr;
    }

    auto const* texture = request->capture->texture();
    if (!texture)
        return nullptr;
    // Cool::Log::ToUser::console().remove(request->_iderror_cannot_find_webcam);
    return &*texture;
}

void TextureLibrary_FromWebcam::on_frame_begin()
{
    for (auto& request : _requests)
        request.has_been_requested_this_frame = false;
}

void TextureLibrary_FromWebcam::on_frame_end()
{
    std::erase_if(_requests, [](WebcamRequest const& request) { return !request.has_been_requested_this_frame; });
}

auto TextureLibrary_FromWebcam::has_active_webcams() const -> bool
{
    return !_requests.empty();
}

auto TextureLibrary_FromWebcam::error_from(std::string const& /*webcam_name*/) const -> std::optional<std::string>
{
    // if (index > _requests.size()) // TODO(TD) un test mieux ?
    // {
    //     return fmt::format("Failed to read node from Camera {}:\n", index);
    // }
    // TODO il faut return les erreurs ici, plutôt que de gérer nous même un error_id
    return std::nullopt;
}

void TextureLibrary_FromWebcam::invalidate_request(std::string const& webcam_name)
{
    auto* request = get_request(webcam_name);
    if (request)
        request->capture = nullptr;
}

} // namespace Cool