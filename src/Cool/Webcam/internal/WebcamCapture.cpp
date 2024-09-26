#include "WebcamCapture.h"
#include <opencv2/opencv.hpp>
#include <wcam/wcam.hpp>
#include "Cool/DebugOptions/DebugOptions.h"
#include "Cool/Gpu/Texture.h"
#include "Cool/Log/ToUser.h"
#include "Cool/TextureSource/set_texture_from_image.h"

namespace Cool {

WebcamCapture::~WebcamCapture()
{
    _wants_to_stop_thread.store(true);
    _thread.join();
}

static auto create_opencv_capture(size_t webcam_index) -> cv::VideoCapture
{
    auto capture = cv::VideoCapture{static_cast<int>(webcam_index)};
    if (capture.isOpened())
    {
        capture.setExceptionMode(true);
        capture.set(cv::CAP_PROP_FRAME_WIDTH, 100);
        capture.set(cv::CAP_PROP_FRAME_HEIGHT, 100);
    }
    return capture;
}

void WebcamCapture::thread_job(WebcamCapture& This)
{
    try
    {
        cv::VideoCapture capture = create_opencv_capture(This.webcam_index());
        cv::Mat          wip_image{};
        while (!This._wants_to_stop_thread.load() && capture.isOpened())
        {
            capture >> wip_image;

            std::scoped_lock lock{This._mutex};
            cv::swap(This._available_image, wip_image);
            This._needs_to_create_texture_from_available_image = true;
        }
    }
    catch (...)
    {
    }
    This._has_stopped = true;
}

auto WebcamCapture::texture() -> Cool::Texture const*
{
    update_webcam_ifn();
    if (!_texture)
        return nullptr;
    return &*_texture;
}

void WebcamCapture::update_webcam_ifn()
{
    std::scoped_lock lock{_mutex};
    if (!_needs_to_create_texture_from_available_image)
        return;

    set_texture_from_opencv_image(_texture, _available_image);
    if (DebugOptions::log_when_creating_textures())
        Log::ToUser::info("Webcam", fmt::format("Generated texture for webcam {}", _webcam_index));
    _needs_to_create_texture_from_available_image = false;
}

} // namespace Cool