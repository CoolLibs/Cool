#include "Exporter.h"
#include <Cool/File/File.h>
#include <Cool/ImGui/ImGuiExtras.h>
#include <Cool/Log/ToUser.h>
#include <Cool/Path/Path.h>
#include "ExporterU.h"

namespace Cool {

Exporter::Exporter()
    : _folder_path_for_image{Path::root() / "out"}
    , _folder_path_for_video{Path::root() / "exports"}
{
    _image_export_window.on_open().subscribe([&](auto) {
        _file_name = File::find_available_name(_folder_path_for_image, _file_name, ".png");
    });
}

void Exporter::set_aspect_ratio(AspectRatio aspect_ratio)
{
    _export_size.set_aspect_ratio(aspect_ratio);
}

void Exporter::maybe_set_aspect_ratio(std::optional<AspectRatio> aspect_ratio)
{
    if (aspect_ratio)
        set_aspect_ratio(*aspect_ratio);
}

void Exporter::imgui_windows(Polaroid polaroid, float time)
{
    imgui_window_export_image(polaroid, time);
    imgui_window_export_video();
}

auto Exporter::output_path() -> std::filesystem::path
{
    return _folder_path_for_image / _file_name.replace_extension("png");
}

void Exporter::imgui_menu_items(imgui_menu_items_Params p)
{
    // Calculate max button width
    const char* longuest_text = "Video";
    float       button_width  = ImGui::CalcTextSize(longuest_text).x + 2.f * ImGui::GetStyle().FramePadding.x;
    // Draw buttons
    if (ImGui::Button("Image", ImVec2(button_width, 0.0f)))
    {
        if (p.open_image_exporter)
            (*p.open_image_exporter)();
        else
        {
            _image_export_window.open();
        }
    }
    if (ImGui::Button("Video", ImVec2(button_width, 0.0f)))
    {
        if (p.open_video_exporter)
            (*p.open_video_exporter)();
        else
        {
            _video_export_window.open();
        }
    }
}

void Exporter::imgui_window_export_image(Polaroid polaroid, float time)
{
    _image_export_window.show([&]() {
        _export_size.imgui();
        // File and Folders
        ImGuiExtras::file("File Name", &_file_name);
        ImGuiExtras::folder("Folder", &_folder_path_for_image);
        // Warning file exists
        ImGui::NewLine();
        if (File::exists(output_path()))
        {
            ImGuiExtras::warning_text("This file already exists. Are you sure you want to overwrite it?");
        }
        // Validation
        if (ImGui::Button("Export as PNG"))
        {
            _image_export_window.close();
            ExporterU::export_image(_export_size, time, polaroid, output_path());
        }
    });
}

auto Exporter::clear_export_folder() const -> bool
{
    if (!File::exists(_folder_path_for_video)
        || std::filesystem::is_empty(_folder_path_for_video))
        return true; // Nothing to do, there was no previous content

    if (boxer::show(fmt::format("You are about the delete all the previous content of {}.\nAre you sure?", _folder_path_for_video).c_str(), "Overwriting previous export", boxer::Style::Warning, boxer::Buttons::OKCancel)
        != boxer::Selection::OK)
        return false; // User doesn't want to delete the previous content of the export folder

    std::filesystem::remove_all(_folder_path_for_video);
    return true;
}

void Exporter::begin_video_export()
{
    if (!clear_export_folder())
        return;

    if (File::create_folders_if_they_dont_exist(_folder_path_for_video))
        _video_export_process.emplace(_video_export_params, _folder_path_for_video, _export_size);
    else
        Log::ToUser::warning("Exporter::begin_video_export", "Couldn't start exporting because folder creation failed!");
}

void Exporter::update(Polaroid polaroid)
{
    if (_video_export_process.has_value())
    {
        if (_video_export_process->update(polaroid))
        {
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
    if (is_exporting())
    {
        ImGui::Begin("Video export in progress");
        _video_export_process->imgui();
        ImGui::End();
    }
    else
    {
        _video_export_window.show([&]() {
            _export_size.imgui();
            ImGuiExtras::folder("Folder", &_folder_path_for_video);
            _video_export_params.imgui();
            // Validation
            if (ImGui::Button("Start exporting"))
            {
                _video_export_window.close();
                begin_video_export();
            }
        });
    }
}

} // namespace Cool