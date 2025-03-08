#include "ExporterU.h"
#include <memory>
#include <open/open.hpp>
#include "Cool/File/File.h"
#include "Cool/Image/SaveImage.h"
#include "Cool/Task/Task.hpp"
#include "Cool/Task/TaskManager.hpp"
#include "ImGuiNotify/ImGuiNotify.hpp"
#include "Task_SaveImage.hpp"

namespace Cool::ExporterU {

void export_image_using_a_task(img::Size size, Time time, Time delta_time, Polaroid const& polaroid, std::filesystem::path const& file_path)
{
    polaroid.render(size, time, delta_time);
    task_manager().submit(std::make_shared<Task_SaveImage>(file_path, polaroid.texture().download_pixels()));
}

auto user_accepted_to_overwrite_image(std::filesystem::path const& file_path) -> bool
{
    if (!Cool::File::exists(file_path))
        return true;

    return boxer::show(fmt::format("\"{}\" already exists. Are you sure you want to overwrite it?", file_path).c_str(), "Overwrite image?", boxer::Style::Warning, boxer::Buttons::OKCancel)
           == boxer::Selection::OK;
}

auto notification_after_video_export_success(std::filesystem::path const& path) -> ImGuiNotify::Notification
{
    return {
        .type                 = ImGuiNotify::Type::Success,
        .title                = "Video Export Success",
        .custom_imgui_content = [path]() {
            if (ImGui::Button(fmt::format("Open \"{}\" folder", Cool::File::file_name(path)).c_str()))
                open_folder_in_explorer(path);
        },
    };
}

auto notification_after_video_export_failure(std::string const& error_message) -> ImGuiNotify::Notification
{
    return {
        .type    = ImGuiNotify::Type::Error,
        .title   = "Video Export Failed",
        .content = error_message,
    };
}

auto notification_after_video_export_canceled(std::filesystem::path const& path) -> ImGuiNotify::Notification
{
    return {
        .type                 = ImGuiNotify::Type::Warning,
        .title                = "Video Export Canceled",
        .custom_imgui_content = [path]() {
            if (ImGui::Button(fmt::format("Open \"{}\" folder", Cool::File::file_name(path)).c_str()))
                open_folder_in_explorer(path);
        },
    };
}

} // namespace Cool::ExporterU
