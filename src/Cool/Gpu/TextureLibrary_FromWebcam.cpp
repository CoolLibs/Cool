#include "TextureLibrary_FromWebcam.h"
#include <algorithm>
#include <memory>
#include <opencv2/opencv.hpp>
#include <optional>
#include <ranges>
#include <string>
#include <vector>
#include "Cool/Gpu/Texture.h"
#include "Cool/Log/MessageConsole.h"
#include "Cool/Log/ToUser.h"

namespace Cool {

TextureLibrary_FromWebcam::TextureLibrary_FromWebcam()
    : _number_of_webcam(compute_number_of_camera())
{
    for (int i = 0; i < _number_of_webcam; i++)
    {
        // add_webcam();
    }
    add_webcam();
}

auto TextureLibrary_FromWebcam::get_webcam_texture(size_t index) -> std::optional<Texture> const&
{
    if (index >= _list_webcam.size()) //  || !_list_webcam[index]._texture) TODO(TD) need it ?
    {
        return std::nullopt;
    }

    if (_list_webcam[index].is_dirty)
        update_webcam(_list_webcam[index]); // TODO(TD)(à test) check si elle a été déjà update

    return _list_webcam[index]._texture;
}

auto TextureLibrary_FromWebcam::compute_number_of_camera() -> int // code from se0kjun : https://gist.github.com/se0kjun/f4b0fdf395181b495f79
{
    int              maxTested = 10;
    cv::VideoCapture temp_camera;

    for (int i = 0; i < maxTested; i++)
    {
        temp_camera.open(i);
        if (!temp_camera.isOpened())
        {
            return i;
        }
    }
    temp_camera.release();
    return maxTested;
}

void TextureLibrary_FromWebcam::add_webcam()
{
    int size = _list_webcam.size();
    int id   = std::min(size, 0); // TODO(TD) id must correspond to the texture index
    _list_webcam.emplace_back(WebcamCapture{
        ._texture = Texture{},
        ._capture = cv::VideoCapture{id},
        ._name    = "Unknown TODO(TD)" + std::to_string(id)});

    // update_webcam(_list_webcam.back());
}

void TextureLibrary_FromWebcam::update_webcams()
{
    for (WebcamCapture& webcam : _list_webcam)
        update_webcam(webcam);
}

void update_webcam(WebcamCapture& webcam)
{
    // if (!webcam._texture.has_value())
    //     return;

    cv::Mat mat;
    webcam._capture >> mat;

    const auto width  = static_cast<unsigned int>(mat.cols);
    const auto height = static_cast<unsigned int>(mat.rows);

    if (!webcam._texture)
        webcam._texture = Texture{{width, height}, 3, reinterpret_cast<uint8_t*>(mat.ptr())};

    else
    {
        webcam._texture->set_image(
            {width, height},
            reinterpret_cast<uint8_t*>(mat.ptr()),
            {
                .internal_format = glpp::InternalFormat::RGBA,
                .channels        = glpp::Channels::RGB,
                .texel_data_type = glpp::TexelDataType::UnsignedByte,
            }
        );
    }
    webcam.is_dirty = false; // the webcam is now up to date
}

void TextureLibrary_FromWebcam::on_frame_begin()
{
    for (auto& webcam_capture : _list_webcam)
        webcam_capture.is_dirty = true;
}

void TextureLibrary_FromWebcam::on_frame_end() // destroy the texture if it has not been updated during the frame
{
    for (auto& webcam_capture : _list_webcam)
        if (webcam_capture.is_dirty == true)
            webcam_capture._texture.reset();
}

auto TextureLibrary_FromWebcam::imgui_widget_webcam_index(int& webcam_index) -> bool
{
    bool        b = false;
    std::string text;
    for (int i = 0; i < _number_of_webcam; i++)
    {
        text.append(std::to_string(i));
        text.push_back('\0');
    }
    text.push_back('\0');

    b |= ImGui::Combo("Camera Displayed", &webcam_index, text.c_str());
    return b;
}

auto TextureLibrary_FromWebcam::has_active_webcam() const -> bool // true if at least one Texture has been updated
{
    return std::ranges::any_of(_list_webcam.begin(), _list_webcam.end(), [](const WebcamCapture& webcam) { return webcam._texture.has_value(); });
}

auto TextureLibrary_FromWebcam::error_from(const size_t index) const -> std::optional<std::string>
{
    if (index > _list_webcam.size()) // TODO(TD) un test mieux ?
    {
        return fmt::format("Failed to read node from Camera {}:\n", index);
    }
    return std::nullopt;
}

} // namespace Cool