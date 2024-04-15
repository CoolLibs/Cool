#include "VideoPlayer.h"
#include <exception>
#include <opencv2/videoio.hpp>
#include <string>
#include <tl/expected.hpp>
#include "Cool/DebugOptions/DebugOptions.h"
#include "Cool/File/File.h"
#include "Cool/Log/ToUser.h"
#include "Cool/TextureSource/set_texture_from_opencv_image.h"
#include "Cool/Time/time_formatted_hms.h"

namespace Cool {

// TODO(Video) Don't load file unless actually used in current graph, to avoid having many captures that are not needed (although OpenCV's capture system might be smart enough to not take too much memory while not requested?)

// TODO(Video):
// cv::redirectError([](int status, const char* func_name,
//                      const char* err_msg, const char* file_name,
//                      int line, void* userdata) {
//     Cool::Log::ToUser::warning("OpenCV", err_msg);
//     return 1;
// });
// cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_VERBOSE);

void VideoPlayer::set_path(std::filesystem::path path)
{
    _path = std::move(path);
    create_capture();
}

void VideoPlayer::create_capture()
{
    auto maybe = internal::CaptureState::create(_path);
    if (maybe.has_value())
    {
        _capture_state = std::move(*maybe);
        _error_message.reset();
    }
    else
    {
        _capture_state.reset();
        _error_message = fmt::format("Failed to open video file: {}", maybe.error());
    }
}

auto internal::CaptureState::create(std::filesystem::path const& path) -> tl::expected<internal::CaptureState, std::string>
{
    try
    {
        auto res     = internal::CaptureState{};
        res._capture = cv::VideoCapture{path.string()};
        if (!res._capture.isOpened())
        {
            return tl::make_unexpected(
                File::exists(path)
                    ? fmt::format("The file is not a valid video file ({})", path)
                    : fmt::format("The file does not exist ({})", path)
            );
        }
        res._capture.setExceptionMode(true);

        res._frames_per_second = res._capture.get(cv::CAP_PROP_FPS);
        res._frames_count      = static_cast<int>(res._capture.get(cv::CAP_PROP_FRAME_COUNT));
        if (res._frames_count <= 0)
            return tl::make_unexpected(fmt::format("Empty video ({} frames)", res._frames_count));
        if (res._frames_per_second <= 0.)
            return tl::make_unexpected(fmt::format("Invalid framerate ({})", res._frames_per_second));

        return res;
    }
    catch (std::exception const& e)
    {
        return tl::make_unexpected(e.what());
    }
}

auto VideoPlayer::get_texture(float time_in_seconds) -> Texture const*
{
    if (!_capture_state.has_value())
        return nullptr;

    _error_message.reset(); // Clear message from previous failure of get_texture(), we will re-add one if an error gets thrown again.
    try
    {
        auto const* res = &_capture_state->get_texture(time_in_seconds);
        if (DebugOptions::log_when_creating_textures())
            Log::ToUser::info("Video File", fmt::format("Generated texture for {} at {}", _path, time_formatted_hms(time_in_seconds, true /*show_milliseconds*/)));
        return res;
    }
    catch (std::exception const& e)
    {
        _error_message = e.what();
        return nullptr;
    }
}

auto internal::CaptureState::get_texture(float time_in_seconds) -> Texture const&
{
    // TODO(Video) Respect looping option, and handle out of bounds properly (return a transparent texture when looping is None).
    auto const desired_frame = static_cast<int>(std::floor(time_in_seconds * _frames_per_second)) % _frames_count; // TODO(Video) Handle start_time, speed, and looping options
    if (_texture.has_value() && _frame_in_texture == desired_frame)
        return *_texture;

    if (_next_frame_in_capture > desired_frame)
    {
        // TODO(Video) Implement this to improve performance when playing video backward: https://www.opencv-srf.com/2017/12/play-video-file-backwards.html
        _capture.set(cv::CAP_PROP_POS_FRAMES, static_cast<double>(desired_frame)); // TODO(Video) is it more performant to set CAP_PROP_POS_MSEC?
        _next_frame_in_capture = desired_frame;
    }
    if (desired_frame - _next_frame_in_capture > 10) // TODO(Video) When is it better to do this than get frames one by one ? 10 is probably not the right number.
    {
        _capture.set(cv::CAP_PROP_POS_FRAMES, static_cast<double>(desired_frame)); // TODO(Video) is it more performant to set CAP_PROP_POS_MSEC?
        _next_frame_in_capture = desired_frame;
    }
    cv::Mat image{};
    while (_next_frame_in_capture < desired_frame)
    {
        // _capture->grab();
        _capture >> image; // TODO(Video) Is it faster to call grab??
        _next_frame_in_capture++;
    }

    assert(_next_frame_in_capture == desired_frame);
    _capture >> image;
    _next_frame_in_capture++;
    set_texture_from_opencv_image(_texture, image);
    _frame_in_texture = desired_frame;
    return *_texture;
}

} // namespace Cool