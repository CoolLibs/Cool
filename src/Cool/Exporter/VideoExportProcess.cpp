#include "VideoExportProcess.h"
#include <Cool/ImGui/ImGuiExtras.h>
#include <Cool/String/String.h>
#include "internal/origin_of_frames.h"

namespace Cool {

static auto nb_digits(int n) -> int
{
    return static_cast<int>(std::ceil(std::log10(n)));
}

VideoExportProcess::VideoExportProcess(const VideoExportParams& params, std::filesystem::path folder_path, img::Size size)
    : _total_nb_of_frames_in_sequence{static_cast<int>(std::ceil((params.end - params.beginning) * params.fps))}
    , _frame_numbering_offset{static_cast<int>(std::ceil(params.beginning * params.fps))} // Makes sure than if we export frames from 0 to 10 seconds, and then decide to extend that video and export frames from 10 to 20 seconds, that second batch of frames will have numbers that follow the ones of the first batch, allowing us to create a unified image sequence with numbers that match up.
    , _last_export{std::chrono::steady_clock::now()}
    , _folder_path{folder_path}
    , _size{size}
    , _clock{params.fps, params.beginning}
{
    _thread_pool.start();
}

bool VideoExportProcess::update(Polaroid polaroid)
{
    if (_should_stop_asap || _nb_frames_which_finished_exporting.load() == _total_nb_of_frames_in_sequence)
        return true; // The export is finished

    if (_nb_frames_sent_to_thread_pool == _total_nb_of_frames_in_sequence || !_thread_pool.has_available_worker())
        return false; // The export is not finished but we can't send work to the thread pool right now, or we have already send the last bits of work to the threads and just have to wait for them to finish

    // Actual export of one frame
    export_frame(
        polaroid,
        (_folder_path / String::to_string(
                            _nb_frames_sent_to_thread_pool + _frame_numbering_offset + origin_of_frames, nb_digits(origin_of_frames)
                        ))
            .replace_extension("png")
    );
    _nb_frames_sent_to_thread_pool++;
    _clock.update();

    return false;
}

auto VideoExportProcess::estimated_remaining_time() -> float
{
    const auto nb_frames_to_render = static_cast<float>(_total_nb_of_frames_in_sequence - _nb_frames_sent_to_thread_pool);
    const auto nb_frames_to_export = static_cast<float>(_total_nb_of_frames_in_sequence - _nb_frames_which_finished_exporting.load());
    // return nb_frames_to_render * _rendering_average_time +
    //        nb_frames_to_export * _export_average_time / _thread_pool.size() +
    //        1.f;
    return nb_frames_to_render * _time_between_two_exports +
           _thread_pool.nb_jobs_in_queue() * _export_average_time /* / _thread_pool.size() */
           + 1.f;
}

void VideoExportProcess::imgui()
{
    int frame_count = _nb_frames_which_finished_exporting.load();
    ImGui::Text(
        "%s",
        ("Exported " +
         String::to_string(frame_count, nb_digits(_total_nb_of_frames_in_sequence)) +
         " / " +
         std::to_string(_total_nb_of_frames_in_sequence) +
         " frames"
        )
            .c_str()
    );
    ImGuiExtras::time_formated_hms(estimated_remaining_time());
    ImGui::SameLine();
    ImGui::Text("remaining");
    ImGui::Text("Time to Render: %.3f", (float)_rendering_average_time);
    ImGui::Text("Time to Export: %.3f", (float)_export_average_time);
    ImGui::Text("Time between Exports: %.3f", (float)_time_between_two_exports);
    ImGui::Text("Last Time between Exports: %.3f", _last_dt);
    if (ImGui::Button("Stop exporting"))
    {
        _should_stop_asap = true;
    }
}

void VideoExportProcess::export_frame(Polaroid polaroid, std::filesystem::path file_path)
{
    {
        const auto begin = std::chrono::steady_clock::now();

        polaroid.render(_clock.time(), _size);
        const auto                   end        = std::chrono::steady_clock::now();
        std::chrono::duration<float> delta_time = end - begin;
        _rendering_average_time.push(delta_time.count());
    }

    _thread_pool.push_job(ImageExportJob{
        file_path,
        polaroid.render_target.download_pixels(),
        _export_average_time,
        _export_average_time_mutex,
        _nb_frames_which_finished_exporting});

    {
        const auto                   end        = std::chrono::steady_clock::now();
        std::chrono::duration<float> delta_time = end - _last_export;
        _last_export                            = end;

        static bool first = true;
        if (first)
        {
            first = false;
            return;
        }
        _time_between_two_exports.push(delta_time.count());
        _last_dt = delta_time.count();
    }
}

} // namespace Cool
