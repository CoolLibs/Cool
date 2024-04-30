#include "VideoPlayer.h"
#include <chrono>
#include <easy_ffmpeg/src/VideoDecoder.hpp>
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
#include "Cool/NfdFileFilter/NfdFileFilter.h"
#include "Cool/TextureSource/set_texture_from_opencv_image.h"
#include "Cool/Time/time_formatted_hms.h"
#include "hack_get_global_time_in_seconds.h"
#include "smart/smart.hpp"

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

// TODO(Video) gets very laggy when two videos are open at once. We might be seeking too much ? Or maybe its only a pb when one of the videos is not actually played ? We need to not send it at all, not even load it

auto VideoPlayerSettings::imgui_widget() -> bool
{
    bool b = false;
    // b |= ImGuiExtras::toggle("Does loop", &do_loop);
    b |= ImGui::Combo("Loop mode", reinterpret_cast<int*>(&loop_mode), "None\0Loop\0Hold\0\0"); // NOLINT(*reinterpret-cast)
    b |= ImGui::DragFloat("Playback speed", &playback_speed, 0.0005f, 0.f, FLT_MAX, "x%.2f", ImGuiSliderFlags_NoRoundToFormat | ImGuiSliderFlags_Logarithmic);
    b |= ImGuiExtras::drag_time("Start time", &start_time);
    return b;
}

auto VideoPlayer::imgui_widget() -> bool
{
    bool b = false;
    ImGui::SeparatorText("Select File");
    if (ImGuiExtras::file_and_folder("File", &_path, NfdFileFilter::Video))
    {
        b = true;
        set_path(_path);
    }
    if (_capture_state.has_value())
    {
        ImGui::NewLine();
        ImGuiExtras::help_marker(fmt::format("(Click to copy)\n\n{}", _capture_state->detailed_video_info()).c_str());
        if (ImGui::IsItemHovered())
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        if (ImGui::IsItemClicked())
            ImGui::SetClipboardText(_capture_state->detailed_video_info().c_str());
    }
    ImGui::NewLine();
    ImGui::SeparatorText("Playback options");
    b |= _settings.imgui_widget();
    return b;
}

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
        _error_message = fmt::format("Failed to open video file {}:\n{}", _path, maybe.error());
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

auto VideoPlayer::get_texture(float time_in_seconds) -> Texture const*
{
    if (!_capture_state.has_value())
        return nullptr;

    _error_message.reset(); // Clear message from previous failure of get_texture(), we will re-add one if an error gets thrown again.
    try
    {
        return &_capture_state->get_texture(time_in_seconds, _settings, _path);
    }
    catch (std::exception const& e)
    {
        _error_message = e.what();
        return nullptr;
    }
}

auto internal::CaptureState::get_texture(float time_in_seconds, VideoPlayerSettings const& settings, std::filesystem::path const& path) -> Texture const&
{
    // TODO(Video) Test the various Settings
    double desired_time = (time_in_seconds - settings.start_time) * settings.playback_speed;
    switch (settings.loop_mode)
    {
    case VideoPlayerLoopMode::None:
    {
        if (desired_time < 0. || desired_time >= _capture->duration_in_seconds())
            return transparent_texture();
        break;
    }
    case VideoPlayerLoopMode::Loop:
    {
        desired_time = smart::mod(desired_time, _capture->duration_in_seconds());
        break;
    }
    case VideoPlayerLoopMode::Hold:
    {
        desired_time = std::clamp(desired_time, 0., _capture->duration_in_seconds());
        break;
    }
    }
    // if (_texture.has_value() && _frame_in_texture == desired_frame)
    //     return *_texture;

    // if (_next_frame_in_capture > desired_frame)
    // {
    //     // TODO debug option to log when seeking
    //     Cool::Log::Debug::info("Video", "Seeking");
    //     // TODO(Video) Implement this to improve performance when playing video backward: https://www.opencv-srf.com/2017/12/play-video-file-backwards.html
    //     _capture.seek_to(time_in_seconds * 1'000'000'000); // TODO(Video) is it more performant to set CAP_PROP_POS_MSEC?
    //     _next_frame_in_capture = desired_frame;
    // }
    // if (desired_frame - _next_frame_in_capture > 10) // TODO(Video) When is it better to do this than get frames one by one ? 10 is probably not the right number.
    // {
    //     Cool::Log::Debug::info("Video", "Seeking");
    //     _capture.seek_to(time_in_seconds * 1'000'000'000); // TODO(Video) is it more performant to set CAP_PROP_POS_MSEC?
    //     _next_frame_in_capture = desired_frame;
    // }

    // Cool::Log::Debug::info("Test", std::to_string((float)_capture.current_frame().pts * (float)_capture.video_stream().time_base.num / (float)_capture.video_stream().time_base.den));

    // while (_next_frame_in_capture < desired_frame)
    // {
    //     // _capture->grab();
    //     _capture.move_to_next_frame();
    //     ; // TODO(Video) Is it faster to call grab??
    //     _next_frame_in_capture++;
    // }

    // assert(_next_frame_in_capture == desired_frame);
    // _capture.move_to_next_frame();
    // _next_frame_in_capture++;
    set_texture_from_ffmpeg_image(_texture, _capture->get_frame_at(time_in_seconds, hack_get_is_dragging_time_in_the_timeline() ? ffmpeg::SeekMode::Fast : ffmpeg::SeekMode::Fast)); // TODO use Exact mode while exporting
    // _frame_in_texture = desired_frame;
    if (DebugOptions::log_when_creating_textures())
        Log::ToUser::info("Video File", fmt::format("Generated texture for {} at {}", path, time_formatted_hms(time_in_seconds, true /*show_milliseconds*/)));

    return *_texture;
}

} // namespace Cool