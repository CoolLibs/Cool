#include "TextureLibrary_FromWebcam.h"
#include <algorithm>
#include <memory>
#include <optional>
#include <vector>
#include "Cool/Gpu/Texture.h"
#include "Cool/Log/MessageConsole.h"
#include "Cool/Log/ToUser.h"
#include "opencv2/core/mat.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/videoio.hpp"

namespace Cool {

static auto gen_dummy_texture() -> Texture // TODO(TD) mettre en commun avec la librarie de webcam
{
    return Texture{img::Size{1, 1}, 3, std::array<uint8_t, 3>{255, 0, 255}.data()};
}

TextureLibrary_FromWebcam::TextureLibrary_FromWebcam()
    : _number_of_webcam(compute_number_of_camera())
{
    for (int i = 0; i < _number_of_webcam; i++)
    {
        add_webcam();
    }
}

auto TextureLibrary_FromWebcam::get_webcam_texture(size_t index) -> Texture const&
{
    if (index >= _list_webcam.size())
    {
        static auto const dummy_texture = gen_dummy_texture();
        Cool::Log::ToUser::console().send(
            Message{
                .category = "Nodes",
                .message  = fmt::format("Failed to read node from Camera {}:\n", index),
                .severity = MessageSeverity::Warning,
            }
        );
        return dummy_texture; // TODO(TD) return the dummy texture and send an error message (cf what TextureLibrary_FromFile does)
    }

    if (_list_webcam[index].is_dirty)
        update_webcam(_list_webcam[index]); // TODO(TD) check si elle a été déjà update
    return _list_webcam[index]._texture.value();
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
    int id = std::max(_number_of_webcam - 1, 0); // TODO id must correspond to the texture index
    _list_webcam.emplace_back(WebcamCapture{
        ._texture = Texture{},
        ._capture = cv::VideoCapture{id},
        ._name    = "Unknown TODO(TD)"});

    // update_webcam(_list_webcam.back());
}

void TextureLibrary_FromWebcam::update_webcams()
{
    for (WebcamCapture& webcam : _list_webcam)
        update_webcam(webcam);
}

void update_webcam(WebcamCapture& webcam)
{
    if (!webcam._texture.has_value())
        return;

    cv::Mat mat;
    webcam._capture >> mat;

    const auto width  = static_cast<unsigned int>(mat.cols);
    const auto height = static_cast<unsigned int>(mat.rows);

    webcam._texture->set_image(
        {width, height},
        3, reinterpret_cast<uint8_t*>(mat.ptr())
    );
    webcam.is_dirty = false; // the webcam is now up to date
}

void TextureLibrary_FromWebcam::on_frame_begin()
{
    for (auto& webcam_capture : _list_webcam)
        webcam_capture.is_dirty = true;
}

void TextureLibrary_FromWebcam::on_frame_end()
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

} // namespace Cool