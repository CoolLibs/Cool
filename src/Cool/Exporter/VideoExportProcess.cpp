#include "VideoExportProcess.h"
#include <Cool/ImGui/Fonts.h>
#include <Cool/ImGui/ImGuiExtras.h>
#include <Cool/String/String.h>
#include "Cool/Time/time_formatted_hms.h"
#include "internal/origin_of_frames.h"

namespace Cool {

static auto nb_digits(int64_t n) -> int
{
    return static_cast<int>(std::ceil(std::log10(n)));
}

VideoExportProcess::VideoExportProcess(VideoExportParams const& params, TimeSpeed time_speed, std::filesystem::path const& folder_path, img::Size size)
    : _folder_path{folder_path}
    , _size{size}
    , _clock{params.fps}
    , _total_nb_of_frames_in_sequence{static_cast<int64_t>(std::ceil((params.end - params.beginning).as_seconds_double() * params.fps))}
    , _frame_numbering_offset{static_cast<int64_t>(std::ceil(params.beginning.as_seconds_double() * params.fps))} // Makes sure than if we export frames from 0 to 10 seconds, and then decide to extend that video and export frames from 10 to 20 seconds, that second batch of frames will have numbers that follow the ones of the first batch, allowing us to create a unified image sequence with numbers that match up.
{
    _clock.set_time(params.beginning, true /* force_delta_time_to_ignore_the_change */);
    _clock.time_speed().value() = time_speed;
    _thread_pool.start();
}

bool VideoExportProcess::update(Polaroid const& polaroid)
{
    if (_should_stop_asap || _nb_frames_which_finished_exporting.load() == _total_nb_of_frames_in_sequence)
        return true; // The export is finished

    if (_nb_frames_sent_to_thread_pool == _total_nb_of_frames_in_sequence || !_thread_pool.has_available_worker())
        return false; // The export is not finished but we can't send work to the thread pool right now, or we have already send the last bits of work to the threads and just have to wait for them to finish

    // Actual export of one frame
    export_frame(
        polaroid,
        (_folder_path / String::to_string(_nb_frames_sent_to_thread_pool + _frame_numbering_offset + origin_of_frames, nb_digits(origin_of_frames))).replace_extension("png")
    );
    _nb_frames_sent_to_thread_pool++;
    _clock.update();
    update_time_estimate();

    return false;
}

void VideoExportProcess::update_time_estimate()
{
    auto const now        = std::chrono::steady_clock::now();
    auto const delta_time = Time{now - _last_render};
    _last_render          = now;

    if (_nb_frames_sent_to_thread_pool < 3 * static_cast<int64_t>(_thread_pool.size())) // Ignore the first few frames, as their timing isn't representative (the queue of the thread pool isn't full yet so exporting goes faster)
        return;                                                                         // Technically this should be 2 * _thread_pool.size() (the time to give a job to each thread + fill the queue) but we use 3 to give us some margin, because pushing wrong numbers into our average messes it up for a while, whereas waiting a little longer before we start having an estimate is not a big deal.
    _average_time_between_two_renders.push(delta_time.as_seconds_double());
}

auto VideoExportProcess::estimated_remaining_time() -> Time
{
    auto const nb_frames_to_render = static_cast<double>(_total_nb_of_frames_in_sequence - _nb_frames_sent_to_thread_pool);

    return Time::seconds(
        nb_frames_to_render * _average_time_between_two_renders
        + (static_cast<double>(_thread_pool.nb_jobs_in_queue()) + static_cast<double>(_thread_pool.size()) / 2.) * _average_export_time / static_cast<double>(_thread_pool.size())
        + 1.
    );
}

void VideoExportProcess::imgui(std::function<void()> const& extra_widgets)
{
    auto const frame_count = _nb_frames_which_finished_exporting.load();

    // Progress bar
    float const progress = static_cast<float>(frame_count) / static_cast<float>(_total_nb_of_frames_in_sequence);
    ImGui::ProgressBar(progress, ImVec2(-1.f, 0.f));

    ImGui::PushFont(Font::monospace());
    // Frames count
    ImGui::TextUnformatted(
        fmt::format(
            "{} / {} frames",
            String::to_string(frame_count, nb_digits(_total_nb_of_frames_in_sequence)),
            _total_nb_of_frames_in_sequence
        )
            .c_str()
    );

    // Remaining time
    ImGui::TextUnformatted(time_formatted_hms(estimated_remaining_time()).c_str());
    ImGui::SameLine();
    ImGui::TextUnformatted("remaining");
    ImGui::PopFont();

    // Stop exporting
    if (ImGui::Button("Stop exporting"))
    {
        _should_stop_asap = true;
    }

    extra_widgets();
}

void VideoExportProcess::export_frame(Polaroid const& polaroid, std::filesystem::path const& file_path)
{
    polaroid.render(_size, _clock.time(), _clock.delta_time());

    _thread_pool.push_job(ImageExportJob{
        file_path,
        polaroid.texture.download_pixels(),
        _average_export_time,
        _average_export_time_mutex,
        _nb_frames_which_finished_exporting
    });
}

} // namespace Cool
