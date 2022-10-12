#include "VideoExportProcess.h"
#include <Cool/ImGui/ImGuiExtras.h>
#include <Cool/String/String.h>

namespace Cool {

static constexpr int origin_of_frames = 50000000; // Helps with exporting negative frames. Say we already exported frames 0 to 100
                                                  // and then decide that we want to see a bit of the animation before these frames, e.g. exporting frames from -50 to -1.
                                                  // If we name these image files -50.png, -49.png, etc. the alphabetical order will be reversed and most tools won't handle this properly.
                                                  // This is why we don't want negative numbers in our image file names. We therefore have to offset all the frames, in case
                                                  // we might later want to export frames before the current ones.
                                                  // This number, 50000000, allows for 115 hours of content at 120 FPS. This should be way more than enough.

static auto nb_digits(int n) -> int
{
    return static_cast<int>(std::ceil(std::log10(n)));
}

VideoExportProcess::VideoExportProcess(const VideoExportParams& params, std::filesystem::path folder_path, img::Size size)
    : _total_nb_of_frames_in_sequence{static_cast<int>(std::ceil((params.end - params.beginning) * params.fps))}
    , _frame_numbering_offset{static_cast<int>(std::ceil(params.beginning * params.fps))} // Makes sure than if we export frames from 0 to 10 seconds, and then decide to extend that video and export frames from 10 to 20 seconds, that second batch of frames will have numbers that follow the ones of the first batch, allowing us to create a unified image sequence with numbers that match up.
    , _folder_path{folder_path}
    , _size{size}
    , _clock{params.fps, params.beginning}
{
    _thread_pool.start();
}

bool VideoExportProcess::update(Polaroid polaroid)
{
    if (!_should_stop_asap && _nb_frames_which_finished_exporting.load() < _total_nb_of_frames_in_sequence)
    {
        if (_nb_frames_sent_to_thread_pool < _total_nb_of_frames_in_sequence && _thread_pool.has_available_worker())
        {
            export_frame(
                polaroid,
                (_folder_path / String::to_string(
                                    _nb_frames_sent_to_thread_pool + _frame_numbering_offset + origin_of_frames, nb_digits(origin_of_frames)
                                ))
                    .replace_extension("png")
            );
            _nb_frames_sent_to_thread_pool++;
            _clock.update();
        }
        return false;
    }
    else
    {
        return true;
    }
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
    ImGuiExtras::time_formated_hms(static_cast<float>(_total_nb_of_frames_in_sequence - frame_count) * _frame_time_average + 1.f);
    ImGui::SameLine();
    ImGui::Text("remaining");
    if (ImGui::Button("Stop exporting"))
    {
        _should_stop_asap = true;
    }
}

void VideoExportProcess::export_frame(Polaroid polaroid, std::filesystem::path file_path)
{
    polaroid.render(_clock.time(), _size);
    _thread_pool.push_job(ImageExportJob{
        file_path,
        polaroid.render_target.download_pixels(),
        &_frame_time_average,
        &_nb_frames_which_finished_exporting});
}

} // namespace Cool
