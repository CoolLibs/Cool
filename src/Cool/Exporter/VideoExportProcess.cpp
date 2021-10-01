#include "VideoExportProcess.h"
#include <Cool/ImGui/ImGuiExtras.h>
#include <Cool/String/String.h>

namespace Cool {

VideoExportProcess::VideoExportProcess(const VideoExportParams& params, std::string_view folder_path, ImageSize size)
    : _folder_path{folder_path}
    , _size{size}
    , _total_nb_of_frames_in_sequence{static_cast<int>(std::ceil((params.end - params.beginning) * params.fps))}
    , _max_nb_digits_of_frame_count{static_cast<int>(std::ceil(std::log10(_total_nb_of_frames_in_sequence)))}
    , _clock{params.fps, params.beginning}
{
}

bool VideoExportProcess::update(Polaroid polaroid)
{
    if (!_should_stop_asap && _nb_frames_which_finished_exporting.load() < _total_nb_of_frames_in_sequence) {
        if (_nb_frames_sent_to_thread_pool < _total_nb_of_frames_in_sequence && _thread_pool.has_available_worker()) {
            export_frame(polaroid, _folder_path + "/" + String::to_string(_nb_frames_sent_to_thread_pool, _max_nb_digits_of_frame_count) + ".png");
            _nb_frames_sent_to_thread_pool++;
            _clock.update();
        }
        return false;
    }
    else {
        return true;
    }
}

void VideoExportProcess::imgui()
{
    int frame_count = _nb_frames_which_finished_exporting.load();
    ImGui::Text(("Exported " + String::to_string(frame_count, _max_nb_digits_of_frame_count) + " / " + std::to_string(_total_nb_of_frames_in_sequence) + " frames").c_str());
    ImGuiExtras::time_formated_hms((_total_nb_of_frames_in_sequence - frame_count) * _frame_time_average / _thread_pool.size());
    ImGui::SameLine();
    ImGui::Text("remaining");
    if (ImGui::Button("Stop exporting")) {
        _should_stop_asap = true;
    }
}

void VideoExportProcess::export_frame(Polaroid polaroid, std::string_view file_path)
{
    // Render
    polaroid.render(_size, _clock.time());
    // Write png
    _thread_pool.push_job(ImageExportJob{
        file_path,
        polaroid.render_target.download_pixels(),
        &_frame_time_average,
        &_nb_frames_which_finished_exporting});
}

} // namespace Cool
