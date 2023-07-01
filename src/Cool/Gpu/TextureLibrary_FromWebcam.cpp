#include "TextureLibrary_FromWebcam.h"
#include <algorithm>
#include <memory>
#include <vector>
#include "Cool/Gpu/Texture.h"
#include "opencv2/core/mat.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/videoio.hpp"

namespace Cool {

TextureLibrary_FromWebcam::TextureLibrary_FromWebcam()
    : _number_of_webcam(1)
{
    for (int i = 0; i < _number_of_webcam; i++)
    {
        add_webcam();
    }
    _list_webcam[0]._is_active = true;
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

void TextureLibrary_FromWebcam::update()
{
    // if (_number_of_webcam < 1)
    // {
    //     _number_of_webcam++;
    //     add_webcam();
    // }
    std::cout << "update cam POUR DE VRAI / ";

    const int diff = _number_of_webcam - _list_webcam.size(); // check if there is closed or open cameras

    update_webcams();
}

void TextureLibrary_FromWebcam::add_webcam()
{
    int              id = std::max(_number_of_webcam - 1, 0);
    cv::VideoCapture cam{id};
    cv::Mat          image;
    cam >> image;

    const auto width  = static_cast<unsigned int>(image.cols);
    const auto height = static_cast<unsigned int>(image.rows);

    // Do i need to fill the texture at first ?
    _list_webcam.push_back(Webcam{
        ._texture = std::make_shared<Cool::Texture>(Cool::Texture{{width, height}, 3, reinterpret_cast<uint8_t*>(image.ptr()), {.interpolation_mode = glpp::Interpolation::NearestNeighbour}}),
        ._capture = cam,
        ._name    = cam.getBackendName()});
}
void TextureLibrary_FromWebcam::update_webcams()

{
    for (Webcam& webcam : _list_webcam)
        update_webcam(webcam);
}

void update_webcam(Webcam& webcam)
{
    if (!webcam._is_active)
        return;

    cv::Mat mat;
    webcam._capture >> mat;

    const auto width  = static_cast<unsigned int>(mat.cols);
    const auto height = static_cast<unsigned int>(mat.rows);

    webcam._texture->set_image({
                                   width,
                                   height,
                               },
                               3, reinterpret_cast<uint8_t*>(mat.ptr()));
}

auto TextureLibrary_FromWebcam::imgui_widget_webcam_index(int& webcam_index) -> bool
{
    bool        b = false;
    std::string text;
    for (int i = 0; i < 1; i++) // TODO(TD) utiliser nombre de webcam depuis la webcam library
    {
        text.append(std::to_string(i));
        text.push_back('\0');
    }

    b |= ImGui::Combo("Camera Displayed", &webcam_index, text.c_str());
    return b;
}

} // namespace Cool