#include "VideoPlayer.h"
#include <chrono>
#include <exception>
#include <opencv2/videoio.hpp>
#include <string>
#include <tl/expected.hpp>
#include "Cool/DebugOptions/DebugOptions.h"
#include "Cool/File/File.h"
#include "Cool/Gpu/default_textures.h"
#include "Cool/ImGui/ImGuiExtras.h"
#include "Cool/Log/Debug.h"
#include "Cool/Log/ToUser.h"
#include "Cool/TextureSource/set_texture_from_opencv_image.h"
#include "Cool/Time/time_formatted_hms.h"
#include "hack_get_global_time_in_seconds.h"
#include "smart/smart.hpp"

namespace Cool {

// TODO(Video):
// cv::redirectError([](int status, const char* func_name,
//                      const char* err_msg, const char* file_name,
//                      int line, void* userdata) {
//     Cool::Log::ToUser::warning("OpenCV", err_msg);
//     return 1;
// });
// cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_VERBOSE);

VideoPlayer::VideoPlayer(VideoDescriptor desc)
    : _desc{std::move(desc)}
{
    create_capture();
}

void VideoPlayer::create_capture()
{
    auto maybe = internal::CaptureState::create(path());
    if (maybe.has_value())
    {
        _capture_state = std::move(*maybe);
        _error_message.reset();
    }
    else
    {
        _capture_state.reset();
        _error_message = fmt::format("Failed to open video file {}:\n{}", path(), maybe.error());
    }
}

auto internal::CaptureState::create(std::filesystem::path const& path) -> tl::expected<internal::CaptureState, std::string>
{
    try
    {
        return internal::CaptureState{path};
    }
    catch (std::exception const& e)
    {
        return tl::make_unexpected(e.what());
    }
}

internal::CaptureState::CaptureState(std::filesystem::path const& path)
    : _capture{std::make_unique<ffmpeg::VideoDecoder>(path, AV_PIX_FMT_RGBA)}
{}

auto VideoPlayer::get_texture(double time_in_seconds) -> Texture const*
{
    if (!_capture_state.has_value())
        return nullptr;

    _error_message.reset(); // Clear message from previous failure of get_texture(), we will re-add one if an error gets thrown again.
    try
    {
        return &_capture_state->get_texture(time_in_seconds, settings(), path());
    }
    catch (std::exception const& e)
    {
        _error_message = e.what();
        return nullptr;
    }
}

auto VideoPlayer::get_current_texture() const -> Texture const*
{
    if (!_capture_state.has_value())
        return nullptr;
    auto const& maybe = _capture_state->get_current_texture();
    if (!maybe.has_value())
        return nullptr;
    return &*maybe;
}

auto VideoPlayer::detailed_video_info() const -> std::string const*
{
    if (!_capture_state.has_value())
        return nullptr;
    return &_capture_state->detailed_video_info();
}

auto internal::CaptureState::get_texture(double time_in_seconds, VideoPlayerSettings const& settings, std::filesystem::path const& path) -> Texture const&
{
    // TODO(Video) Implement this to improve performance when playing video backward: https://www.opencv-srf.com/2017/12/play-video-file-backwards.html
    time_in_seconds = (time_in_seconds - settings.start_time) * settings.playback_speed;
    switch (settings.loop_mode)
    {
    case VideoPlayerLoopMode::None:
    {
        if (time_in_seconds < 0. || time_in_seconds >= _capture->duration_in_seconds())
            return transparent_texture();
        break;
    }
    case VideoPlayerLoopMode::Loop:
    {
        time_in_seconds = smart::mod(time_in_seconds, _capture->duration_in_seconds());
        break;
    }
    case VideoPlayerLoopMode::Hold:
    {
        time_in_seconds = std::clamp(time_in_seconds, 0., _capture->duration_in_seconds());
        break;
    }
    }

    // if (_texture.has_value() && _frame_in_texture == desired_frame)
    //     return *_texture;

    // if (_next_frame_in_capture > desired_frame)
    // {
    //     // TODO debug option to log when seeking
    //     Cool::Log::Debug::info("Video", "Seeking");
    //     _capture.seek_to(time_in_seconds * 1'000'000'000);
    //     _next_frame_in_capture = desired_frame;
    // }
    // if (desired_frame - _next_frame_in_capture > 10)
    // {
    //     Cool::Log::Debug::info("Video", "Seeking");
    //     _capture.seek_to(time_in_seconds * 1'000'000'000);
    //     _next_frame_in_capture = desired_frame;
    // }

    // Cool::Log::Debug::info("Test", std::to_string((float)_capture.current_frame().pts * (float)_capture.video_stream().time_base.num / (float)_capture.video_stream().time_base.den));

    // while (_next_frame_in_capture < desired_frame)
    // {
    //     // _capture->grab();
    //     _capture.move_to_next_frame();
    //     _next_frame_in_capture++;
    // }

    // assert(_next_frame_in_capture == desired_frame);
    // _capture.move_to_next_frame();
    // _next_frame_in_capture++;
    set_texture_from_ffmpeg_image(_texture, _capture->get_frame_at(time_in_seconds, hack_get_is_dragging_time_in_the_timeline() ? ffmpeg::SeekMode::Fast : ffmpeg::SeekMode::Fast)); // TODO use Exact mode while exporting
    // _frame_in_texture = desired_frame;
    if (DebugOptions::log_when_creating_textures())
        Log::ToUser::info("Video File", fmt::format("Generated texture for {} at {}", path, time_formatted_hms(static_cast<float>(time_in_seconds), true /*show_milliseconds*/)));

    return *_texture;
}

} // namespace Cool