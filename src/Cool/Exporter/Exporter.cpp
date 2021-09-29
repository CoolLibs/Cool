#include "Exporter.h"
#include <Cool/File/File.h>
#include <Cool/ImGuiExtras/ImGuiExtras.h>
#include <Cool/Image/ImageSizeU.h>
#include <Cool/Log/ToUser.h>
#include <Cool/String/String.h>
#include <Cool/Time/Time.h>
#include "ExporterU.h"

namespace Cool {

Exporter::Exporter()
    : _folder_path_for_image{File::root_dir() + "/out"}
    , _folder_path_for_image_sequence{File::root_dir() + "/exports"}
{
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
            ExporterU::export_image(_export_size, input, output_path());
        }
        //
        ImGui::End();
    }
}

void Exporter::begin_image_sequence_export()
{
    if (File::create_folders_if_they_dont_exist(_folder_path_for_image_sequence)) {
        _video_export_process.emplace(_video_export_params, _folder_path_for_image_sequence, _export_size);
    }
    else {
        Log::ToUser::warn("Exporter::begin_image_sequence_export", "Couldn't start exporting because folder creation failed !");
    }
}

void Exporter::update(ExporterInput input)
{
    if (_video_export_process.has_value()) {
        _is_window_open_image_sequence_export = true;
        if (_video_export_process->update(input)) {
            end_image_sequence_export();
        }
    }
}

void Exporter::end_image_sequence_export()
{
    _video_export_process.reset();
    _is_window_open_image_sequence_export = false;
}

bool Exporter::imgui_window_export_image_sequence()
{
    bool has_started = false;
    if (_is_window_open_image_sequence_export) {
        ImGui::Begin("Export an Image Sequence", is_exporting() ? nullptr : &_is_window_open_image_sequence_export);
        // Not exporting
        if (!is_exporting()) {
            ImageSizeU::imgui(_export_size);
            ImGui::InputText("Path", &_folder_path_for_image_sequence);
            ImGui::SameLine();
            ImGuiExtras::open_folder_dialog(&_folder_path_for_image_sequence, _folder_path_for_image_sequence);
            _video_export_params.imgui();
            // Validation
            if (ImGui::Button("Start exporting")) {
                has_started = true;
                begin_image_sequence_export();
            }
        }
        // Exporting
        else {
            _video_export_process->imgui();
        }
        ImGui::End();
    }
    return has_started;
}

} // namespace Cool