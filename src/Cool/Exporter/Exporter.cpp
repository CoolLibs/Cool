#include "Exporter.h"
#include <Cool/File/File.h>
#include <Cool/ImGuiExtras/ImGuiExtras.h>
#include <Cool/Image/ImageSizeU.h>
#include <Cool/Log/ToUser.h>
#include <Cool/String/String.h>
#include <Cool/Time/Time.h>

namespace Cool {

Exporter::Exporter()
    : _folder_path_for_image(File::root_dir() + "/out")
    , _folder_path_for_image_sequence(File::root_dir() + "/exports")
{
}

void Exporter::export_image(ExporterInput in, std::string_view file_path)
{
    // Render
    auto prev_constrained_size    = in.render_target.constrained_size();
    auto restore_constrained_size = sg::make_scope_guard([&]() { in.render_target.set_constrained_size(prev_constrained_size); });
    in.render_target.set_constrained_size(_export_size);
    in.render_fn(in.render_target);
    // Get data
    const auto image = in.render_target.download_pixels();
    // Write png
    if (File::create_folders_if_they_dont_exist(_folder_path_for_image)) {
        ExportImage::as_png(file_path, image.width(), image.height(), image.data.get());
    }
    else {
        Log::ToUser::warn("Exporter::export_image", "Failed to create folder \"{}\"", _folder_path_for_image);
    }
}

void Exporter::export_image_multithreaded(ExporterInput in, std::string_view file_path)
{
    // Render
    in.render_target.set_constrained_size(_export_size);
    in.render_fn(in.render_target);
    // Wait for a thread to be available
    _thread_pool.wait_for_available_thread();
    // Write png
    _thread_pool.push_job(ExportImage_Functor{
        file_path,
        in.render_target.download_pixels(),
        &_frame_time_average,
        &_nb_frames_which_finished_exporting});
}

std::string Exporter::output_path()
{
    return _folder_path_for_image + "/" + _file_name + ".png";
}

void Exporter::find_available_file_name()
{
    if (File::exists(output_path())) {
        // Find base_name and k
        int         k         = 1;
        std::string base_name = _file_name;
        size_t      pos       = _file_name.find_last_of("(");
        if (pos != std::string::npos) {
            // Find number in parenthesis
            base_name      = _file_name.substr(0, pos);
            size_t end_pos = _file_name.find_last_of(")");
            try {
                k = std::stoi(_file_name.substr(pos + 1, end_pos - pos));
            }
            catch (std::exception e) {
                k         = 1;
                base_name = _file_name;
            }
        }
        // Find available name
        while (File::exists(output_path())) {
            _file_name = base_name + "(" + std::to_string(k) + ")";
            k++;
        }
    }
}

void Exporter::open_window_export_image(bool open)
{
    _is_window_open_image_export = open;
    if (_is_window_open_image_export) {
        find_available_file_name();
    }
}

void Exporter::imgui_menu_items()
{
    // Calculate max button width
    const char* longuest_text = "Image Sequence";
    float       button_width  = ImGui::CalcTextSize(longuest_text).x + 2.f * ImGui::GetStyle().FramePadding.x;
    // Draw buttons
    //ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.5f, 0.5f));
    if (ImGui::Button("Image", ImVec2(button_width, 0.0f))) {
        open_window_export_image(true);
    }
    if (ImGui::Button("Image Sequence", ImVec2(button_width, 0.0f))) {
        _is_window_open_image_sequence_export = true;
    }
    //ImGui::PopStyleVar();
}

void Exporter::imgui_window_export_image(ExporterInput input)
{
    if (_is_window_open_image_export) {
        ImGui::Begin("Export an Image", &_is_window_open_image_export);
        // Resolution
        ImageSizeU::imgui(_export_size);
        // File and Folders
        bool path_has_changed = false;
        path_has_changed |= ImGui::InputText("File Name", &_file_name);
        path_has_changed |= ImGui::InputText("Path", &_folder_path_for_image);
        ImGui::SameLine();
        path_has_changed |= ImGuiExtras::open_folder_dialog(&_folder_path_for_image, _folder_path_for_image);
        if (path_has_changed) {
            _should_show_file_exists_warning = File::exists(output_path());
        }
        // Warning file exists
        ImGui::NewLine();
        if (_should_show_file_exists_warning) {
            ImGuiExtras::warning_text("This file already exists. Are you sure you want to overwrite it ?");
        }
        // Validation
        if (ImGui::Button("Export as PNG")) {
            _is_window_open_image_export = false;
            export_image(input, output_path());
        }
        //
        ImGui::End();
    }
}

void Exporter::begin_image_sequence_export()
{
    if (File::create_folders_if_they_dont_exist(_folder_path_for_image_sequence)) {
        _thread_pool.start();
        _is_exporting_image_sequence        = true;
        _nb_frames_sent_to_thread_pool      = 0;
        _nb_frames_which_finished_exporting = 0;
        float total_export_duration         = _sequence_end_time_in_sec - _sequence_begin_time_in_sec;
        _total_nb_of_frames_in_sequence     = static_cast<unsigned int>(std::ceil(total_export_duration * _fps));
        _max_nb_digits_of_frame_count       = static_cast<int>(std::ceil(std::log10(_total_nb_of_frames_in_sequence)));
        Time::set_elapse_mode_as_fixed_timestep(_fps);
        Time::set_time(_sequence_begin_time_in_sec);
        _frame_time_average.clear();
    }
    else {
        Log::ToUser::warn("Exporter::begin_image_sequence_export", "Couldn't start exporting because folder creation failed !");
    }
}

void Exporter::update(ExporterInput input)
{
    if (_is_exporting_image_sequence) {
        _is_window_open_image_sequence_export = true;
        if (_nb_frames_which_finished_exporting.load() < _total_nb_of_frames_in_sequence) {
            if (_nb_frames_sent_to_thread_pool < _total_nb_of_frames_in_sequence) {
                export_image_multithreaded(input, _folder_path_for_image_sequence + "/" + String::to_string(_nb_frames_sent_to_thread_pool, _max_nb_digits_of_frame_count) + ".png");
                _nb_frames_sent_to_thread_pool++;
            }
        }
        else {
            end_image_sequence_export();
        }
    }
}

void Exporter::end_image_sequence_export()
{
    _thread_pool.stop();
    _is_exporting_image_sequence = false;
    Time::set_elapse_mode_as_realtime();
    _is_window_open_image_sequence_export = false;
}

bool Exporter::imgui_window_export_image_sequence()
{
    bool has_started = false;
    if (_is_window_open_image_sequence_export) {
        ImGui::Begin("Export an Image Sequence", _is_exporting_image_sequence ? nullptr : &_is_window_open_image_sequence_export);
        // Not exporting
        if (!_is_exporting_image_sequence) {
            ImageSizeU::imgui(_export_size);
            ImGui::InputText("Path", &_folder_path_for_image_sequence);
            ImGui::SameLine();
            ImGuiExtras::open_folder_dialog(&_folder_path_for_image_sequence, _folder_path_for_image_sequence);
            ImGui::InputFloat("FPS", &_fps);
            ImGui::PushItemWidth(50);
            ImGui::Text("From");
            ImGui::SameLine();
            ImGui::PushID(13540);
            ImGui::DragFloat("", &_sequence_begin_time_in_sec);
            ImGui::PopID();
            ImGui::SameLine();
            ImGui::Text("To");
            ImGui::SameLine();
            ImGui::PushID(14540);
            ImGui::DragFloat("", &_sequence_end_time_in_sec);
            ImGui::PopID();
            ImGui::SameLine();
            ImGui::Text("seconds");
            ImGui::PopItemWidth();
            // Validation
            if (ImGui::Button("Start exporting")) {
                _is_exporting_image_sequence = true;
                has_started                  = true;
                begin_image_sequence_export();
            }
        }
        // Exporting
        else {
            int frame_count = _nb_frames_which_finished_exporting.load();
            ImGui::Text(("Exported " + String::to_string(frame_count, _max_nb_digits_of_frame_count) + " / " + std::to_string(_total_nb_of_frames_in_sequence) + " frames").c_str());
            ImGuiExtras::time_formated_hms((_total_nb_of_frames_in_sequence - frame_count) * _frame_time_average / _thread_pool.size());
            ImGui::SameLine();
            ImGui::Text("remaining");
            if (ImGui::Button("Stop exporting")) {
                end_image_sequence_export();
            }
        }
        ImGui::End();
    }
    return has_started;
}

} // namespace Cool