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
#include <utility>
#include <vector>
#include <webcam_info/webcam_info.hpp>
#include <xmemory>
#include "Cool/Gpu/Texture.h"
#include "Cool/Gpu/TextureLibrary_FromWebcam.h"
#include "Cool/ImGui/ImGuiExtras.h"
#include "Cool/Log/MessageConsole.h"
#include "Cool/Log/ToUser.h"
#include "fmt/core.h"

// windows media fondation

namespace Cool {

static auto get_all_webcams() -> std::vector<webcam_info::info>
{
    auto list_webcams_infos = webcam_info::get_all_webcams();
    for (auto& webcam_info : list_webcams_infos)
    {
        std::sort(webcam_info.list_resolution.begin(), webcam_info.list_resolution.end(), [](webcam_info::resolution& res_a, webcam_info::resolution& res_b) {
            return (res_a.width > res_b.width)
                   || ((res_a.width == res_b.width) && res_a.height > res_b.height);
        });
        webcam_info.list_resolution.erase(
            std::unique(
                webcam_info.list_resolution.begin(),
                webcam_info.list_resolution.end()
            ),
            webcam_info.list_resolution.end()
        );
    }
    return list_webcams_infos;
}

void TextureLibrary_FromWebcam::thread_webcams_infos_works(const std::stop_token& stop_token, TextureLibrary_FromWebcam& This)
{
    std::vector<webcam_info::info> wip_webcams_infos{};
    while (!stop_token.stop_requested())
    {
        wip_webcams_infos = get_all_webcams();
        {
            std::scoped_lock<std::mutex> lock(This._mutex_webcam_info);
            std::swap(wip_webcams_infos, This._webcams_infos);
        }
    }

} // namespace Cool

void WebcamCapture::thread_webcam_work(const std::stop_token& stop_token, WebcamCapture& This, size_t webcam_index)
{
    auto set_capture_parameters = [&](int width, int height, cv::VideoCapture& capture) {
        capture.setExceptionMode(true);
        if (capture.isOpened())
        {
            capture.set(cv::CAP_PROP_FRAME_WIDTH, width);
            capture.set(cv::CAP_PROP_FRAME_HEIGHT, height);
            capture.set(cv::CAP_PROP_FPS, 30);
        }
    };

    cv::VideoCapture capture{static_cast<int>(webcam_index)};
    cv::Mat          wip_image{};

    int  width{330};
    int  height{100};
    auto resolution = TextureLibrary_FromWebcam::instance().get_resolution_from_index(webcam_index);
    if (resolution.has_value())
    {
        width  = resolution->width;
        height = resolution->height;
    }

    try
    {
        set_capture_parameters(width, height, capture);
    }
    catch (cv::Exception& e)
    {
        // TODO
    }

    while (!stop_token.stop_requested())
    {
        try
        {
            if (!capture.isOpened())
            {
                This.error = "Failed to open Webcam";
                capture    = cv::VideoCapture{static_cast<int>(webcam_index)};
                // set_capture_parameters(); TODO() Do this
                std::this_thread::sleep_for(1s);
                return;
            }
            else
            {
                capture >> wip_image;
                std::scoped_lock lock(This._mutex);
                cv::swap(This._available_image, wip_image);
                This.is_dirty = true;
            }

            This.error.reset();
        }

        catch (cv::Exception& e)
        {
            This.error = "Failed to open Webcam";
            capture    = cv::VideoCapture{static_cast<int>(webcam_index)};
            set_capture_parameters(width, height, capture);
        }
    }
}

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

auto TextureLibrary_FromWebcam::get_index_from_name(const std::string name) -> std::optional<size_t>
{
    std::scoped_lock<std::mutex> lock(_mutex_webcam_info);
    for (size_t i = 0; i < _webcams_infos.size(); i++)
    {
        if (_webcams_infos[i].name == name)
            return i;
    }
    return std::nullopt;
}

auto TextureLibrary_FromWebcam::get_name_from_index(size_t index) -> std::optional<std::string>
{
    std::scoped_lock<std::mutex> lock(_mutex_webcam_info);
    if (index >= _webcams_infos.size())
        return std::nullopt;
    return _webcams_infos[index].name;
}

auto TextureLibrary_FromWebcam::get_resolution_from_index(size_t index) -> std::optional<webcam_info::resolution>
{
    auto possible_name = get_name_from_index(index);
    if (!possible_name.has_value())
        return std::nullopt;
    return get_config_from_name(*possible_name).resolution;
}
auto TextureLibrary_FromWebcam::get_default_resolution_from_name(const std::string& name) -> std::optional<webcam_info::resolution>
{
    for (auto& infos : _webcams_infos)
    {
        if (infos.name == name)
        {
            if (infos.list_resolution.empty())
                return std::nullopt;
            return infos.list_resolution[0];
        }
    }
    return std::nullopt;
}

// auto TextureLibrary_FromWebcam::get_resolution_from_name(const std::string& name) -> webcam_info::resolution
// {
//     return _list_webcam_configs.find(name)->second.resolution;
// }

auto TextureLibrary_FromWebcam::get_config_from_name(const std::string& name) -> WebcamConfig&
{
    auto config = _list_webcam_configs.find(name);
    if (config == _list_webcam_configs.end())
    {
        return _list_webcam_configs.emplace(name, WebcamConfig{get_default_resolution_from_name(name).value_or(webcam_info::resolution{})}).first->second;
    }
    return config->second;
}

auto TextureLibrary_FromWebcam::get_request_from_name(const std::string& name) -> WebcamRequest*
{
    for (auto& request : _requests)
    {
        if (request._name == name)
            return &request;
    }
    return nullptr;
}

auto TextureLibrary_FromWebcam::get_webcam_texture(const std::string name) -> Texture const*
{
    WebcamRequest* request = get_request(name);
    auto           index   = get_index_from_name(name);
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
                    .category = "Nodes",
                    .message  = fmt::format("cannot find webcam {}", name),
                    .severity = MessageSeverity::Error,
                }
            );
        request->_capture.reset();
        return nullptr;
    }

    if (!request->_capture || request->_capture->_webcam_index != *index)
    {
        request->create_capture(*index);
    }

    if (request->_capture->error)
    {
        Cool::Log::ToUser::console()
            .send(
                request->_iderror_cannot_find_webcam,
                Message{
                    .category = "Nodes",
                    .message  = fmt::format("{} {}, Maybe it is used in an other sofware", *request->_capture->error, request->_name),
                    .severity = MessageSeverity::Error,
                }
            );
        request->_capture.reset();
        return nullptr;
    }

    update_webcam_ifn(*request->_capture);
    Cool::Log::ToUser::console().remove(request->_iderror_cannot_find_webcam);

    if (!request->_capture->_texture)
        return nullptr;
    return &*request->_capture->_texture;
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

//         auto it = std::find_if(_requests.begin(), _requests.end(), [index](WebcamCapture const& webcam) { return (webcam._webcam_index == index); });

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

// void TextureLibrary_FromWebcam::add_webcam(const int index, const std::string name)
// {
//     try
//     {
//         _requests.emplace_back(index, name);
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
    if (webcam_name.empty()) // HACK to make sure a newly created webcam variable has a valid webcam name.
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

    if (ImGuiExtras::button_with_text_icon(ICOMOON_COG))
    {
        _webcam_config_window.open();
    }
    ImGuiExtras::tooltip("Open Webcams Config to choose the resolutions");
    // ImGui::SameLine();

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

void TextureLibrary_FromWebcam::open_webcams_config_window()
{
    _webcam_config_window.open();
}

void TextureLibrary_FromWebcam::imgui_windows()
{
    _webcam_config_window.show([this]() {
        std::scoped_lock<std::mutex> lock(this->_mutex_webcam_info);
        for (auto& infos : this->_webcams_infos)
        {
            auto& config = get_config_from_name(infos.name);
            // auto        webcam_resolution   = std::make_pair(infos.resolution.width, infos.height);
            std::string combo_preview_value = fmt::format("{} x {}", config.resolution.width, config.resolution.height);

            if (ImGui::BeginCombo(infos.name.c_str(), combo_preview_value.c_str(), 0))
            {
                for (auto& resolution : infos.list_resolution)
                {
                    const bool  is_selected     = (config.resolution == resolution);
                    std::string resolution_name = fmt::format("{} x {}", resolution.width, resolution.height);

                    if (ImGui::Selectable(resolution_name.c_str(), is_selected))
                    {
                        config.resolution = resolution;

                        auto* request = get_request_from_name(infos.name);
                        if (request)
                            request->_capture = nullptr;

                        // Trouver la requete correspodnante si elle existe, et si oui détruire sa capture car elle n'est plus valide
                    }

                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
                // ImGui::SameLine();
                // ImGui::Text("{}\n", infos.name.c_str());
            }
        }
    });
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

auto TextureLibrary_FromWebcam::get_default_webcam_name() -> std::string
{
    std::scoped_lock<std::mutex> lock(_mutex_webcam_info);

    return _webcams_infos.empty() ? "" : _webcams_infos[0].name;
}

void WebcamRequest::create_capture(const size_t index)
{
    _capture = std::make_unique<WebcamCapture>(index);
}

} // namespace Cool