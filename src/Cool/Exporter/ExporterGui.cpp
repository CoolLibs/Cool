#include "ExporterGui.h"
#include <Cool/File/File.h>
#include <Cool/ImGui/Fonts.h>
#include <Cool/ImGui/ImGuiExtras.h>
#include <Cool/Log/ToUser.h>
#include <Cool/Path/Path.h>
#include <imgui.h>
#include "Cool/ImGui/icon_fmt.h"
#include "ExporterU.h"

namespace Cool {

ExporterGui::ExporterGui()
    : _folder_path_for_image{Path::root() / "out"}
    , _folder_path_for_video{Path::root() / "exports"}
{
    _image_export_window.on_open().subscribe([&](auto) {
        _file_name = File::find_available_name(_folder_path_for_image, _file_name, ".png");
    });
}

void ExporterGui::set_aspect_ratio(AspectRatio const& aspect_ratio)
{
    _export_size.set_aspect_ratio(aspect_ratio);
}

void ExporterGui::maybe_set_aspect_ratio(std::optional<AspectRatio> const& aspect_ratio)
{
    if (aspect_ratio)
        set_aspect_ratio(*aspect_ratio);
}

void ExporterGui::imgui_windows(Polaroid const& polaroid, float time, std::function<void(std::filesystem::path const&)> const& on_image_exported, std::optional<VideoExportProcess>& video_export_process)
{
    imgui_window_export_image(polaroid, time, on_image_exported);
    imgui_window_export_video(video_export_process);
}

auto ExporterGui::output_path() -> std::filesystem::path
{
    return _folder_path_for_image / _file_name.replace_extension("png");
}

void ExporterGui::imgui_menu_items(imgui_menu_items_Params const& p, std::optional<std::string> longest_text)
{
    // Calculate max button width
    if (!longest_text)
        longest_text = icon_fmt("Video", ICOMOON_FILM, true);
    float const button_width = ImGui::CalcTextSize(longest_text->c_str()).x + 2.f * ImGui::GetStyle().FramePadding.x;
    // Draw buttons
    if (ImGui::Button(icon_fmt("Image", ICOMOON_IMAGE, true).c_str(), ImVec2(button_width, 0.0f)))
    {
        if (p.open_image_exporter)
            (*p.open_image_exporter)();
        else
        {
            _image_export_window.open();
        }
    }
    if (ImGui::Button(icon_fmt("Video", ICOMOON_FILM, true).c_str(), ImVec2(button_width, 0.0f)))
    {
        if (p.open_video_exporter)
            (*p.open_video_exporter)();
        else
        {
            _video_export_window.open();
        }
    }
}

void ExporterGui::imgui_window_export_image(Polaroid polaroid, float time, std::function<void(std::filesystem::path const&)> const& on_image_exported)
{
    _image_export_window.show([&]() {
        _export_size.imgui();
        // File and Folders
        ImGuiExtras::file("File Name", &_file_name, {}, {}, false /*No dialog button*/);
        ImGuiExtras::folder("Folder", &_folder_path_for_image);

        ImGui::SeparatorText("");
        // Warning file exists
        if (File::exists(output_path()))
        {
            ImGuiExtras::warning_text("This file already exists. Are you sure you want to overwrite it?");
        }
        // Validation
        if (ImGui::Button(icon_fmt("Export as PNG", ICOMOON_UPLOAD2).c_str()))
        {
            _image_export_window.close();
            ExporterU::export_image(_export_size, time, polaroid, output_path());
            on_image_exported(output_path());
        }
    });
}

auto ExporterGui::clear_export_folder() const -> bool
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

void ExporterGui::begin_video_export(std::optional<VideoExportProcess>& video_export_process)
{
    if (!clear_export_folder())
        return;

    if (File::create_folders_if_they_dont_exist(_folder_path_for_video))
        video_export_process.emplace(_video_export_params, _folder_path_for_video, _export_size);
    else
        Log::ToUser::warning("ExporterGui::begin_video_export", "Couldn't start exporting because folder creation failed!");
}

void ExporterGui::update(Polaroid const& polaroid, std::optional<VideoExportProcess>& video_export_process)
{
    if (!video_export_process.has_value())
        return;

    if (video_export_process->update(polaroid))
        end_video_export(video_export_process);
}

void ExporterGui::end_video_export(std::optional<VideoExportProcess>& video_export_process)
{
    video_export_process.reset();
}

void ExporterGui::imgui_window_export_video(std::optional<VideoExportProcess>& video_export_process)
{
    if (is_exporting(video_export_process))
    {
        ImGui::Begin("Video export in progress");
        ImGui::PushFont(Font::monospace());
        video_export_process->imgui();
        ImGui::PopFont();
        ImGui::End();
    }
    else
    {
        _video_export_window.show([&]() {
            _export_size.imgui();
            ImGuiExtras::folder("Folder", &_folder_path_for_video);
            _video_export_params.imgui();
            // Validation
            ImGui::SeparatorText("");
            if (ImGui::Button(icon_fmt("Start exporting", ICOMOON_UPLOAD2).c_str()))
            {
                _video_export_window.close();
                begin_video_export(video_export_process);
            }
        });
    }
}

} // namespace Cool