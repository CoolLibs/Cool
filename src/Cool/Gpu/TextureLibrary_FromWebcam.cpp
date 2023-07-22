#include "TextureLibrary_FromWebcam.h"
#include <algorithm>
#include <memory>
#include <mutex>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <optional>
#include <ranges>
#include <shared_mutex>
#include <string>
#include <thread>
#include <vector>
#include "Cool/Gpu/Texture.h"
#include "Cool/Gpu/TextureLibrary_FromWebcam.h"
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

auto TextureLibrary_FromWebcam::get_webcam(const int i) -> WebcamCapture*
{
    for (auto& webcam : _webcams)
    {
        if (webcam._webcam_id == i)
            return &webcam;
    }
    return nullptr;
}

auto TextureLibrary_FromWebcam::get_webcam_texture(size_t index) -> std::optional<Texture> const&
{
    WebcamCapture* cap = get_webcam(index);
    if (cap == nullptr)
        return std::nullopt;

    if (cap->is_dirty)
    {
        update_webcam(*cap); // TODO(TD)(à test) check si elle a été déjà update
    }
    return cap->_texture;
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
    int size = _webcams.size();
    int id   = std::min(size, 0); // TODO(TD) id must correspond to the texture index

    // auto& capture      = _webcams[id];
    // capture._webcam_id = id;
    // capture._name      = "Unknown TODO(TD)" + std::to_string(id);
    // capture._thread    = std::jthread{&WebcamCapture::thread_webcam_work, this, id};
    _webcams.emplace_back(id);
}

void TextureLibrary_FromWebcam::update_webcams() // TODO(TD) ne sert plus à rien ?
{
    for (auto& webcam : _webcams)
    {
        update_webcam(webcam);
    }
}

void update_webcam(WebcamCapture& webcam)
{
    {
        std::scoped_lock lock(webcam._mutex);

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
    }
    webcam.is_dirty = false; // the webcam is now up to date
}

void TextureLibrary_FromWebcam::on_frame_begin()
{
    for (auto& webcam : _webcams)
        webcam.is_dirty = true;
}

void TextureLibrary_FromWebcam::on_frame_end() // destroy the texture if it has not been updated during the frame
{
    for (auto& webcam : _webcams)
        if (webcam.is_dirty == true)
            webcam._texture.reset();
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
    return std::ranges::any_of(_webcams.begin(), _webcams.end(), [](auto const& webcam) { return webcam._texture.has_value(); });
}

auto TextureLibrary_FromWebcam::error_from(const size_t index) const -> std::optional<std::string>
{
    if (index > _webcams.size()) // TODO(TD) un test mieux ?
    {
        return fmt::format("Failed to read node from Camera {}:\n", index);
    }
    return std::nullopt;
}

} // namespace Cool