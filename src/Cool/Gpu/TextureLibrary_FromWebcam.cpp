#include "TextureLibrary_FromWebcam.h"
#include <imgui.h>
#include <algorithm>
#include <cmath>
#include <memory>
#include <mutex>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <optional>
#include <ranges>
#include <shared_mutex>
#include <string>
#include <thread>
#include <vector>
#include <webcam_info/webcam_info.hpp>
#include "Cool/Gpu/Texture.h"
#include "Cool/Gpu/TextureLibrary_FromWebcam.h"
#include "Cool/Log/MessageConsole.h"
#include "Cool/Log/ToUser.h"
#include "fmt/core.h"

// windows media fondation
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <winerror.h>

namespace Cool {

auto TextureLibrary_FromWebcam::get_request(const std::string name) -> WebcamRequest*
{
    std::scoped_lock<std::mutex> lock(_mutex_webcam_info);
    for (auto& webcam_request : _requests)
    {
        if (webcam_request._name == name)
            return &webcam_request;
    }
    return nullptr;
}

auto TextureLibrary_FromWebcam::get_id_from_name(const std::string name) -> std::optional<int>
{
    std::scoped_lock<std::mutex> lock(_mutex_webcam_info);
    for (int i = 0; i < _webcams_infos.size(); i++)
    {
        if (_webcams_infos[i].name == name)
            return i;
    }
    return std::nullopt;
}

auto TextureLibrary_FromWebcam::get_name_from_id(int id) -> std::optional<std::string>
{
    std::scoped_lock<std::mutex> lock(_mutex_webcam_info);
    if (id >= _webcams_infos.size())
        return std::nullopt;
    return _webcams_infos[id].name;
}

auto TextureLibrary_FromWebcam::get_webcam_texture(const std::string name) -> std::optional<Texture> const&
{
    WebcamRequest* request = get_request(name);
    auto           id      = get_id_from_name(name);
    if (request == nullptr)
    {
        _requests.emplace_back(id, name);
        request = &_requests.back();
    }

    request->has_been_requested_this_frame = true;

    if (!id.has_value())
    {
        Cool::Log::ToUser::console()
            .send(
                request->_iderror_cannot_find_webcam,
                Message{
                    .category = "Nodes",
                    .message  = fmt::format("cannot find webcam {}", name),
                    .severity = MessageSeverity::Error,
                }
            );
        request->_capture.reset();
        return std::nullopt;
    }
    Cool::Log::ToUser::console().remove(request->_iderror_cannot_find_webcam);

    if (!request->_capture || request->_capture->_webcam_id != *id)
    {
        request->create_capture(*id);
    }

    update_webcam_ifn(*request->_capture);
    return request->_capture->_texture;
}

// auto TextureLibrary_FromWebcam::find_next_webcam_index(const int start_index) -> int // code from se0kjun : https://gist.github.com/se0kjun/f4b0fdf395181b495f79
// {
//     int              maxTested = 3;
//     cv::VideoCapture temp_camera;

//     auto log_message_if_is_the_same_camera = [](const int index, const int start_index) {
//         if (index == start_index)
//         {
//             Cool::Log::ToUser::console()
//                 .send(
//                     Message{
//                         .category = "Nodes",
//                         .message  = fmt::format("There is only one webcam"),
//                         .severity = MessageSeverity::Warning,
//                     }
//                 );
//         }
//     };

//     for (int i = 0; i < maxTested; i++)
//     {
//         int index = (i + start_index + 1) % maxTested;

//         auto it = std::find_if(_requests.begin(), _requests.end(), [index](WebcamCapture const& webcam) { return (webcam._webcam_id == index); });

//         if (it != _requests.end())
//         {
//             log_message_if_is_the_same_camera(index, start_index);
//             return index;
//         }

//         temp_camera.open(index);
//         if (temp_camera.isOpened())
//         {
//             log_message_if_is_the_same_camera(index, start_index);
//             return index;
//         }
//     }
//     Cool::Log::ToUser::console()
//         .send(
//             Message{
//                 .category = "Nodes",
//                 .message  = fmt::format("No Webcam found"),
//                 .severity = MessageSeverity::Warning,
//             }
//         );
//     return 0;
// }

// void TextureLibrary_FromWebcam::add_webcam(const int id, const std::string name)
// {
//     try
//     {
//         _requests.emplace_back(id, name);
//     }
//     catch (cv::Exception& err)
//     {
//         Cool::Log::ToUser::console()
//             .send(
//                 Message{
//                     .category = "Nodes",
//                     .message  = err.what(),
//                     .severity = MessageSeverity::Error,
//                 }
//             );
//     }
// }

void update_webcam_ifn(WebcamCapture& webcam)
{
    std::scoped_lock lock(webcam._mutex);
    if (!webcam.is_dirty)
    {
        return;
    }

    const auto width  = static_cast<unsigned int>(webcam._available_image.cols);
    const auto height = static_cast<unsigned int>(webcam._available_image.rows);

    if (!webcam._texture)
        webcam._texture = Texture{{width, height}, 3, reinterpret_cast<uint8_t*>(webcam._available_image.ptr())};

    else
    {
        webcam._texture->bind();
        webcam._texture->set_image(
            {width, height},
            reinterpret_cast<uint8_t*>(webcam._available_image.ptr()),
            {
                .internal_format = glpp::InternalFormat::RGBA,
                .channels        = glpp::Channels::RGB,
                .texel_data_type = glpp::TexelDataType::UnsignedByte,
            }
        );
        Cool::Texture::unbind();
    }
    webcam.is_dirty = false; // the webcam is now up to date
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

auto TextureLibrary_FromWebcam::imgui_widget_webcam_name(std::string& webcam_name) -> bool
{
    if (webcam_name.empty())
        webcam_name = get_default_webcam_name();
    bool b = false;

    std::vector<std::string> list_name{};
    list_name.reserve(_webcams_infos.size());
    {
        std::scoped_lock<std::mutex> lock(_mutex_webcam_info);
        for (auto const& infos : _webcams_infos)
        {
            list_name.push_back(infos.name);
        }
    }

    std::string combo_preview_name = webcam_name;

    if (ImGui::BeginCombo("Webcam", combo_preview_name.c_str(), 0))
    {
        for (auto& name : list_name)
        {
            const bool is_selected = (webcam_name == name);
            if (ImGui::Selectable(name.c_str(), is_selected))
                webcam_name = name;

            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    return b;
}

auto TextureLibrary_FromWebcam::has_active_webcam() const -> bool // true if at least one Texture has been updated
{
    return !_requests.empty();
}

auto TextureLibrary_FromWebcam::error_from(const std::string webcam_name) const -> std::optional<std::string>
{
    // if (index > _requests.size()) // TODO(TD) un test mieux ?
    // {
    //     return fmt::format("Failed to read node from Camera {}:\n", index);
    // }
    return std::nullopt;
}

auto TextureLibrary_FromWebcam::get_default_webcam_name() -> std::string
{
    std::scoped_lock<std::mutex> lock(_mutex_webcam_info);

    return _webcams_infos.empty() ? "" : _webcams_infos[0].name;
}

void WebcamRequest::create_capture(const int id)
{
    _capture = std::make_unique<WebcamCapture>(id);
}

} // namespace Cool