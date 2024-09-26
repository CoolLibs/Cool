#include "TextureLibrary_FromWebcam.h"
#include <optional>
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

auto TextureLibrary_FromWebcam::get_request(std::string const& webcam_name) const -> WebcamRequest const*
{
    for (auto const& request : _requests)
    {
        if (request.webcam_name == webcam_name)
            return &request;
    }
    return nullptr;
}

auto TextureLibrary_FromWebcam::get_texture(std::string const& webcam_name) -> Texture const*
{
    auto*      request      = get_request(webcam_name);
    auto const webcam_index = std::make_optional(0); // WebcamsInfos::instance().index(webcam_name);

    if (request == nullptr)
    {
        _requests.emplace_back(webcam_index, webcam_name);
        request = &_requests.back();
    }

    request->has_been_requested_this_frame = true;

    if (!webcam_index.has_value())
    {
        request->error_message = fmt::format("Cannot find webcam \"{}\". It might be unplugged.", webcam_name);
        request->capture.reset();
        return nullptr;
    }

    if (!request->capture || request->capture->webcam_index() != *webcam_index)
    {
        request->capture = std::make_unique<WebcamCapture>(
            *webcam_index
        );
    }

    if (request->capture->has_stopped())
    {
        request->error_message = fmt::format(
            "Failed to capture webcam \"{}\". It might be used in another software",
            webcam_name
        );
        request->capture.reset();
        return nullptr;
    }

    auto const* texture = request->capture->texture();
    if (!texture)
        return nullptr;
    request->error_message.reset();
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

auto TextureLibrary_FromWebcam::error_from(std::string const& webcam_name) const -> std::optional<std::string>
{
    auto const* request = get_request(webcam_name);
    if (!request)
        return std::nullopt;
    return request->error_message;
}

void TextureLibrary_FromWebcam::invalidate_request(std::string const& webcam_name)
{
    auto* request = get_request(webcam_name);
    if (request)
        request->capture = nullptr;
}

} // namespace Cool