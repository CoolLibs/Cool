#include "WebcamCapture.h"
#include <opencv2/opencv.hpp>
#include <webcam_info/webcam_info.hpp>
#include "Cool/Gpu/Texture.h"

namespace Cool {

WebcamCapture::~WebcamCapture()
{
    _wants_to_stop_thread.store(true);
    _thread.join();
}

static auto create_opencv_capture(size_t webcam_index, webcam_info::Resolution resolution) -> cv::VideoCapture
{
    auto capture = cv::VideoCapture{static_cast<int>(webcam_index)};
    if (capture.isOpened())
    {
        capture.setExceptionMode(true);
        capture.set(cv::CAP_PROP_FRAME_WIDTH, resolution.width);
        capture.set(cv::CAP_PROP_FRAME_HEIGHT, resolution.height);
    }
    return capture;
}

void WebcamCapture::thread_job(WebcamCapture& This, webcam_info::Resolution resolution)
{
    try
    {
        cv::VideoCapture capture = create_opencv_capture(This.webcam_index(), resolution);
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

    auto const size = img::Size{
        static_cast<unsigned int>(_available_image.cols),
        static_cast<unsigned int>(_available_image.rows),
    };
    auto const layout = TextureLayout{
        .internal_format = InternalFormat::RGBA,
        .channels        = Channels::RGB,
        .texel_data_type = TexelDataType::UnsignedByte,
    };
    auto const* data = static_cast<uint8_t*>(_available_image.ptr());

    if (!_texture)
    {
        _texture = Texture{size, data, layout};
    }
    else
    {
        _texture->bind();
        _texture->set_image(size, data, layout);
        Cool::Texture::unbind();
    }
    _needs_to_create_texture_from_available_image = false;
}

} // namespace Cool