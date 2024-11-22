#include "ExporterGui.h"
#include <Cool/File/File.h>
#include <Cool/ImGui/ImGuiExtras.h>
#include <Cool/Log/ToUser.h>
#include <Cool/Path/Path.h>
#include <imgui.h>
#include "Cool/ImGui/icon_fmt.h"
#include "Cool/NfdFileFilter/NfdFileFilter.h"
#include "Cool/UserSettings/UserSettings.h"
#include "ExporterU.h"
#include "VideoExportOverwriteBehaviour.h"

namespace Cool {

auto ExporterGui::folder_path_for_video() const -> std::filesystem::path
{
    return Cool::File::weakly_canonical(_folder_path_for_video.value_or(Path::project_folder().value_or(Path::user_data()) / "video frames"));
}

void ExporterGui::imgui_windows(exporter_imgui_windows_Params const& p, std::optional<VideoExportProcess>& video_export_process)
{
    imgui_window_export_image(p.polaroid, p.time, p.delta_time, p.on_image_exported);
    imgui_window_export_video(p.widgets_in_window_video_export_in_progress, p.on_video_export_start, video_export_process, p.time_speed);
}

void ExporterGui::imgui_menu_items(exporter_imgui_menu_items_Params const& p, std::optional<std::string> longest_text)
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

void ExporterGui::imgui_window_export_image(Polaroid polaroid, Time time, Time delta_time, std::function<void(std::filesystem::path const&)> const& on_image_exported)
{
    _image_export_window.show([&](bool is_opening) {
        if (is_opening)
            _image_file = File::find_available_path(_image_file);
        _export_size.imgui();
        // File and Folders
        ImGuiExtras::file_and_folder_saving(_image_file, {".png"}, NfdFileFilter::Png);
        // Validation
        ImGuiExtras::before_export_button(_image_file);
        if (ImGui::Button(icon_fmt("Export", ICOMOON_UPLOAD2).c_str()))
        {
            _image_export_window.close();
            ExporterU::export_image(_export_size, time, delta_time, polaroid, _image_file, on_image_exported);
        }
    });
}

auto ExporterGui::user_accepted_our_frames_overwrite_behaviour() -> bool
{
    if (!File::exists(folder_path_for_video())
        || Cool::File::is_empty(folder_path_for_video()))
    {
        return true; // Nothing to do, no frame is going to be overwritten
    }

    switch (user_settings().video_export_overwrite_behaviour)
    {
    case VideoExportOverwriteBehaviour::AskBeforeCreatingNewFolder:
    case VideoExportOverwriteBehaviour::AlwaysCreateNewFolder:
    {
        auto const new_folder_name = File::find_available_name("", folder_path_for_video(), "");
        if (user_settings().video_export_overwrite_behaviour == VideoExportOverwriteBehaviour::AskBeforeCreatingNewFolder)
        {
            if (boxer::show(fmt::format("There are already some frames in {}.\nDo you want to export in another folder? {}", folder_path_for_video(), new_folder_name).c_str(), "Creating a new export folder", boxer::Style::Warning, boxer::Buttons::OKCancel)
                != boxer::Selection::OK)
            {
                return false;
            }
        }

        _folder_path_for_video = new_folder_name;
        break;
    }
    case VideoExportOverwriteBehaviour::AskBeforeOverwritingPreviousFrames:
    {
        if (boxer::show(fmt::format("You are about to overwrite the frames in {}.\nDo you want to continue?", folder_path_for_video()).c_str(), "Overwriting previous export", boxer::Style::Warning, boxer::Buttons::OKCancel)
            != boxer::Selection::OK)
        {
            return false;
        }
        break;
    }
    case VideoExportOverwriteBehaviour::AlwaysOverwritePreviousFrames:
    {
        break; // Nothing to do
    }
    }

    return true;
}

void ExporterGui::begin_video_export(std::optional<VideoExportProcess>& video_export_process, TimeSpeed time_speed, std::function<void()> const& on_video_export_start)
{
    if (!user_accepted_our_frames_overwrite_behaviour())
        return;

    if (File::create_folders_if_they_dont_exist(folder_path_for_video()))
    {
        video_export_process.emplace(_video_export_params, time_speed, folder_path_for_video(), _export_size);
        on_video_export_start();
    }
    else
    {
        Log::ToUser::warning("ExporterGui::begin_video_export", "Couldn't start exporting because folder creation failed!");
        ExporterU::notification_after_export_failure();
    }
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

void ExporterGui::imgui_window_export_video(std::function<void()> const& widgets_in_window_video_export_in_progress, std::function<void()> const& on_video_export_start, std::optional<VideoExportProcess>& video_export_process, TimeSpeed time_speed)
{
    if (is_exporting(video_export_process))
    {
        ImGui::Begin("Video export in progress");
        video_export_process->imgui(widgets_in_window_video_export_in_progress);
        ImGui::End();
    }
    else
    {
        _video_export_window.show([&](bool /*is_opening*/) {
            _export_size.imgui();
            {
                auto path = folder_path_for_video();
                if (ImGuiExtras::folder("Folder", &path))
                    _folder_path_for_video = path;
            }
            _video_export_params.imgui();
            imgui_widget(user_settings().video_export_overwrite_behaviour);
            // Validation
            ImGuiExtras::before_export_button();
            if (ImGui::Button(icon_fmt("Start exporting", ICOMOON_UPLOAD2).c_str()))
            {
                _video_export_window.close();
                begin_video_export(video_export_process, time_speed, on_video_export_start);
            }
        });
    }
}

auto ExporterGui::image_export_path() -> std::filesystem::path const&
{
    _image_file = File::find_available_path(_image_file);
    return _image_file;
}

} // namespace Cool