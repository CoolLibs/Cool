#include "TextureLibrary_FromWebcam.h"
#include <webcam_info/webcam_info.hpp>
#include "Cool/Gpu/Texture.h"
#include "Cool/Log/MessageConsole.h"
#include "Cool/Log/ToUser.h"
#include "Cool/Webcam/TextureLibrary_FromWebcam.h"
#include "Cool/Webcam/WebcamsConfigs.h"
#include "Cool/Webcam/WebcamsInfos.h"

namespace Cool {

auto TextureLibrary_FromWebcam::instance() -> TextureLibrary_FromWebcam&
{
    static auto inst = TextureLibrary_FromWebcam{};
    return inst;
}

auto TextureLibrary_FromWebcam::get_request(std::string const& webcam_name) -> WebcamRequest*
{
    // std::scoped_lock<std::mutex> lock(_mutex_webcam_info);
    for (auto& webcam_request : _requests)
    {
        if (webcam_request._name == webcam_name)
            return &webcam_request;
    }
    return nullptr;
}

// auto TextureLibrary_FromWebcam::get_resolution_from_name(const std::string& name) -> webcam_info::Resolution
// {
//     return _list_webcam_configs.find(name)->second.resolution;
// }

auto TextureLibrary_FromWebcam::get_webcam_texture(const std::string name) -> Texture const*
{
    WebcamRequest* request = get_request(name);
    auto           index   = WebcamsInfos::instance().index(name);
    if (request == nullptr)
    {
        _requests.emplace_back(index, name);
        request = &_requests.back();
    }

    request->has_been_requested_this_frame = true;

    if (!index.has_value())
    {
        Cool::Log::ToUser::console()
            .send(
                request->_iderror_cannot_find_webcam,
                Message{
                    .category = "Webcam",
                    .message  = fmt::format("cannot find webcam {}", name),
                    .severity = MessageSeverity::Error,
                }
            );
        request->_capture.reset();
        return nullptr;
    }

    if (!request->_capture || request->_capture->webcam_index() != *index)
    {
        request->_capture = std::make_unique<WebcamCapture>(
            *index,
            WebcamsConfigs::instance().selected_resolution(name)
        );
    }

    if (request->_capture->has_stopped())
    {
        Cool::Log::ToUser::console()
            .send(
                request->_iderror_cannot_find_webcam,
                Message{
                    .category = "Webcam",
                    .message  = fmt::format("Failed to capture \"{}\". It might be because it is used in another sofware", request->_name),
                    .severity = MessageSeverity::Error,
                }
            );
        request->_capture.reset();
        return nullptr;
    }

    // update_webcam_ifn(*request->_capture);
    auto capture_texture = request->_capture->texture();

    if (!capture_texture)
        return nullptr;

    Cool::Log::ToUser::console().remove(request->_iderror_cannot_find_webcam);
    return &*capture_texture;
}

void TextureLibrary_FromWebcam::on_frame_begin()
{
    for (auto& webcam : _requests)
    {
        webcam.has_been_requested_this_frame = false;
    }
}

void TextureLibrary_FromWebcam::on_frame_end() // destroy the texture if it has not been updated during the frame
{
    std::erase_if(_requests, [](WebcamRequest const& request) { return !request.has_been_requested_this_frame; });
}

auto TextureLibrary_FromWebcam::has_active_webcam() const -> bool // true if at least one Texture has been updated
{
    return !_requests.empty();
}

auto TextureLibrary_FromWebcam::error_from(const std::string /*webcam_name*/) const -> std::optional<std::string>
{
    // if (index > _requests.size()) // TODO(TD) un test mieux ?
    // {
    //     return fmt::format("Failed to read node from Camera {}:\n", index);
    // }
    // TODO il faut return les erreurs ici, plutôt que de gérer nous même un error_id
    return std::nullopt;
}

void TextureLibrary_FromWebcam::invalidate_request(std::string const& request_name)
{
    auto* request = get_request(request_name);
    if (request)
        request->_capture = nullptr;
}

} // namespace Cool