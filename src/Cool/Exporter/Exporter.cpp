#include "Exporter.h"
#include <Cool/File/File.h>
#include <Cool/ImGui/ImGuiExtras.h>
#include <Cool/Image/ImageSizeU.h>
#include <Cool/Log/ToUser.h>
#include <Cool/String/String.h>
#include <Cool/Time/Time.h>
#include "ExporterU.h"

namespace Cool {

Exporter::Exporter()
    : _folder_path_for_image{File::root_dir() + "/out"}
    , _folder_path_for_video{File::root_dir() + "/exports"}
{
    _image_export_window.on_open().subscribe([&](auto) {
        _file_name = File::find_available_name(_folder_path_for_image, _file_name, ".png");
    });
}

void Exporter::imgui_windows(Polaroid polaroid)
{
    imgui_window_export_image(polaroid);
    imgui_window_export_video();
}

std::string Exporter::output_path()
{
    return _folder_path_for_image + "/" + _file_name + ".png";
}

void Exporter::imgui_menu_items()
{
    // Calculate max button width
    const char* longuest_text = "Video";
    float       button_width  = ImGui::CalcTextSize(longuest_text).x + 2.f * ImGui::GetStyle().FramePadding.x;
    // Draw buttons
    if (ImGui::Button("Image", ImVec2(button_width, 0.0f))) {
        _image_export_window.open();
    }
    if (ImGui::Button("Video", ImVec2(button_width, 0.0f))) {
        _video_export_window.open();
    }
}

void Exporter::imgui_window_export_image(Polaroid polaroid)
{
    _image_export_window.show([&]() {
        // Resolution
        ImageSizeU::imgui(_export_size);
        // File and Folders
        ImGui::InputText("File Name", &_file_name);
        ImGui::InputText("Path", &_folder_path_for_image);
        ImGui::SameLine();
        ImGuiExtras::open_folder_dialog(&_folder_path_for_image, _folder_path_for_image);
        // Warning file exists
        ImGui::NewLine();
        if (File::exists(output_path())) {
            ImGuiExtras::warning_text("This file already exists. Are you sure you want to overwrite it ?");
        }
        // Validation
        if (ImGui::Button("Export as PNG")) {
            _image_export_window.close();
            ExporterU::export_image(_export_size, polaroid, output_path());
        }
    });
}

void Exporter::begin_video_export()
{
    if (File::create_folders_if_they_dont_exist(_folder_path_for_video)) {
        _video_export_process.emplace(_video_export_params, _folder_path_for_video, _export_size);
    }
    else {
        Log::ToUser::warn("Exporter::begin_video_export", "Couldn't start exporting because folder creation failed !");
    }
}

void Exporter::update(Polaroid polaroid)
{
    if (_video_export_process.has_value()) {
        if (_video_export_process->update(polaroid)) {
            end_video_export();
        }
    }
}

void Exporter::end_video_export()
{
    _video_export_process.reset();
}

void Exporter::imgui_window_export_video()
{
    if (is_exporting()) {
        ImGui::Begin("Video export in progress");
        _video_export_process->imgui();
        ImGui::End();
    }
    else {
        _video_export_window.show([&]() {
            ImageSizeU::imgui(_export_size);
            ImGui::InputText("Path", &_folder_path_for_video);
            ImGui::SameLine();
            ImGuiExtras::open_folder_dialog(&_folder_path_for_video, _folder_path_for_video);
            _video_export_params.imgui();
            // Validation
            if (ImGui::Button("Start exporting")) {
                _video_export_window.close();
                begin_video_export();
            }
        });
    }
}

} // namespace Cool