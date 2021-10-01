#include "VideoExportProcess.h"
#include <Cool/ImGui/ImGuiExtras.h>
#include <Cool/String/String.h>

namespace Cool {

VideoExportProcess::VideoExportProcess(const VideoExportParams& params, std::string_view folder_path, ImageSize size)
    : _params{params}
    , _folder_path{folder_path}
    , _size{size}
    , _clock{_params.fps, _params.beginning}
{
    _thread_pool.start();
    _nb_frames_sent_to_thread_pool      = 0;
    _nb_frames_which_finished_exporting = 0;
    float total_export_duration         = _params.end - _params.beginning;
    _total_nb_of_frames_in_sequence     = static_cast<unsigned int>(std::ceil(total_export_duration * _params.fps));
    _max_nb_digits_of_frame_count       = static_cast<int>(std::ceil(std::log10(_total_nb_of_frames_in_sequence)));
    _frame_time_average.clear();
}

VideoExportProcess::~VideoExportProcess()
{
    _thread_pool.stop();
}

bool VideoExportProcess::update(Polaroid polaroid)
{
    if (!_should_stop_asap && _nb_frames_which_finished_exporting.load() < _total_nb_of_frames_in_sequence) {
        if (_nb_frames_sent_to_thread_pool < _total_nb_of_frames_in_sequence) {
            export_frame(polaroid, _folder_path + "/" + String::to_string(_nb_frames_sent_to_thread_pool, _max_nb_digits_of_frame_count) + ".png");
            _nb_frames_sent_to_thread_pool++;
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
    // Wait for a thread to be available
    _thread_pool.wait_for_available_thread();
    // Write png
    _thread_pool.push_job(ImageExportJob{
        file_path,
        polaroid.render_target.download_pixels(),
        &_frame_time_average,
        &_nb_frames_which_finished_exporting});
}

} // namespace Cool
