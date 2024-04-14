#include "VideoPlayer.h"
#include <chrono>
#include <opencv2/videoio.hpp>
#include "Cool/TextureSource/set_texture_from_opencv_image.h"

namespace Cool {

// TODO(Video) Use int64_t instead of int for frames count (? How long of a video can we store in 2^31 framas at 200 frames per second?)

// TODO(Video) Don't load file unless actually used in current graph, to avoid having many captures that are not needed (although OpenCV's capture system might be smart enough to not take too much memory while not requested?)

void VideoPlayer::create_capture()
{
    _capture = cv::VideoCapture{_path.string()};
    if (!_capture->isOpened())
    {
        _capture.reset();
        // TODO(Video) Error message, invalid path
        return;
    }
    _capture->setExceptionMode(true);

    _frames_count = static_cast<int>(_capture->get(cv::CAP_PROP_FRAME_COUNT));
    if (_frames_count <= 0)
    {
        _capture.reset();
        // TODO(Video) Error message, empty video
        return;
    }
    _frames_per_second     = _capture->get(cv::CAP_PROP_FPS);
    _next_frame_in_capture = 0;
}

struct CHocpic {
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    ~CHocpic()
    {
        std::cout << std::chrono::steady_clock::now() - begin << '\n';
    }
};

auto VideoPlayer::get_texture(float time_in_seconds) -> Texture const*
{
    if (!_capture.has_value())
        return nullptr;
    CHocpic bob{};
    // TODO(Video) Always keep the first few frames in memory, so that when the video loops back we don't have to wait to seek to these frames (although might not be necessary because seeking to frame 0 might be very fast)

    int const desired_frame = static_cast<int>(std::floor(time_in_seconds * _frames_per_second)) % _frames_count; // TODO(Video) Handle start_time, speed, and looping options
    if (_current_texture.has_value() && _current_frame == desired_frame)
        return &*_current_texture;

    if (_next_frame_in_capture > desired_frame)
    {
        _capture->set(cv::CAP_PROP_POS_FRAMES, desired_frame); // TODO(Video) is it more performant to set CAP_PROP_POS_MSEC?
        // TODO(Video) Implement this to improve performance when playing video backward: https://www.opencv-srf.com/2017/12/play-video-file-backwards.html
        _next_frame_in_capture = desired_frame;
    }
    cv::Mat wip_image{};
    if (desired_frame - _next_frame_in_capture > 10)
    {
        _capture->set(cv::CAP_PROP_POS_FRAMES, desired_frame);
        _next_frame_in_capture = desired_frame;
    }
    while (_next_frame_in_capture < desired_frame) // TODO(Video) When the gap between _next_frame_in_capture and desired_frame is too big, is it more performant to set CAP_PROP_POS_FRAMES ? Or CAP_PROP_POS_MSEC?²
    {
        // _capture->grab();
        *_capture >> wip_image; // TODO(Video) Is it faster to call grab??
        _next_frame_in_capture++;
    }

    assert(_next_frame_in_capture == desired_frame);
    *_capture >> wip_image;
    _next_frame_in_capture++;
    set_texture_from_opencv_image(_current_texture, wip_image);
    _current_frame = desired_frame;
    return &*_current_texture;
}

} // namespace Cool