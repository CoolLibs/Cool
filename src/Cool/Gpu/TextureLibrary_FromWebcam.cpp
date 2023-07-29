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

// windows media fondation
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <winerror.h>

namespace Cool {

auto TextureLibrary_FromWebcam::get_webcam(const int i) -> WebcamCapture*
{
    for (auto& webcam : _webcams)
    {
        if (webcam._webcam_id == i)
            return &webcam;
    }
    return nullptr; // TODO(TD) pb le display toujours une première fois
}

auto TextureLibrary_FromWebcam::get_webcam_texture(const int index) -> std::optional<Texture> const&
{
    WebcamCapture* cap = get_webcam(index);
    if (cap == nullptr)
    {
        add_webcam(index);
        return std::nullopt;
    }

    cap->has_been_requested_this_frame = true;

    update_webcam_ifn(*cap);

    return cap->_texture;
}

auto TextureLibrary_FromWebcam::find_next_webcam_index(const int start_index) -> int // code from se0kjun : https://gist.github.com/se0kjun/f4b0fdf395181b495f79
{
    int              maxTested = 3;
    cv::VideoCapture temp_camera;

    auto log_message_if_is_the_same_camera = [](const int index, const int start_index) {
        if (index == start_index)
        {
            Cool::Log::ToUser::console()
                .send(
                    Message{
                        .category = "Nodes",
                        .message  = fmt::format("There is only one webcam"),
                        .severity = MessageSeverity::Warning,
                    }
                );
        }
    };

    for (int i = 0; i < maxTested; i++)
    {
        int index = (i + start_index + 1) % maxTested;

        auto it = std::find_if(_webcams.begin(), _webcams.end(), [index](WebcamCapture const& webcam) { return (webcam._webcam_id == index); });

        if (it != _webcams.end())
        {
            log_message_if_is_the_same_camera(index, start_index);
            return index;
        }

        temp_camera.open(index);
        if (temp_camera.isOpened())
        {
            log_message_if_is_the_same_camera(index, start_index);
            return index;
        }
    }
    Cool::Log::ToUser::console()
        .send(
            Message{
                .category = "Nodes",
                .message  = fmt::format("No Webcam found"),
                .severity = MessageSeverity::Warning,
            }
        );
    return 0;
}

void TextureLibrary_FromWebcam::add_webcam(const int id)
{
    try
    {
        _webcams.emplace_back(id);
    }
    catch (cv::Exception& err) // TODO(TD) trouver les bonnes exceptions renvoyées par opencv
    {
        Cool::Log::ToUser::console()
            .send(
                Message{
                    .category = "Nodes",
                    .message  = err.what(),
                    .severity = MessageSeverity::Warning,
                }
            );
    }
}

void update_webcam_ifn(WebcamCapture& webcam)
{
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
}

void TextureLibrary_FromWebcam::on_frame_begin()
{
    for (auto& webcam : _webcams)
    {
        webcam.has_been_requested_this_frame = false;
    }
}

void TextureLibrary_FromWebcam::on_frame_end() // destroy the texture if it has not been updated during the frame
{
    _webcams.remove_if([](WebcamCapture const& webcam) { return !webcam.has_been_requested_this_frame; });
}

auto TextureLibrary_FromWebcam::imgui_widget_webcam_index(int& webcam_index) -> bool
{
    bool b                = false;
    auto list_webcam_info = webcam_info::get_all_webcams();

    std::string combo_string{};
    for (auto const& info : list_webcam_info)
    {
        combo_string.append(info.name + '\0');
    }

    b |= ImGui::Combo("Webcams", &webcam_index, (combo_string + '\0').c_str());

    // bool b = false;
    // if (ImGui::Button("Switch to next webcam"))
    // {
    //     webcam_index = find_next_webcam_index(webcam_index);
    // }
    return b;
}

auto TextureLibrary_FromWebcam::has_active_webcam() const -> bool // true if at least one Texture has been updated
{
    return !_webcams.empty();
}

auto TextureLibrary_FromWebcam::error_from(const int index) const -> std::optional<std::string>
{
    // if (index > _webcams.size()) // TODO(TD) un test mieux ?
    // {
    //     return fmt::format("Failed to read node from Camera {}:\n", index);
    // }
    return std::nullopt;
}

} // namespace Cool