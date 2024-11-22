#include "ExporterU.h"
#include <Cool/File/File.h>
#include <Cool/Image/SaveImage.h>
#include <Cool/Log/ToUser.h>
#include <open/open.hpp>
#include "ImGuiNotify/ImGuiNotify.hpp"
#include "no_sleep/no_sleep.hpp"

namespace Cool::ExporterU {

auto export_image(img::Size size, Time time, Time delta_time, Polaroid const& polaroid, std::filesystem::path const& file_path) -> bool
{
    no_sleep::Scoped disable_sleep{COOL_APP_NAME, COOL_APP_NAME " is exporting an image", no_sleep::Mode::ScreenCanTurnOffButKeepComputing};
    polaroid.render(size, time, delta_time);
    bool const success = ImageU::save_png(file_path, polaroid.texture().download_pixels());
    if (success)
        notification_after_export_success(file_path, false);
    else
        notification_after_export_failure();
    return success;
}

void notification_after_export_success(std::filesystem::path const& path, bool is_video)
{
    ImGui::Notify::add({
        .type                 = ImGui::Notify::ToastType::Success,
        .title                = "Export Success",
        .custom_imgui_content = [=]() {
            ImGui::TextUnformatted(Cool::File::file_name(path).string().c_str());
            if (ImGui::Button("Open folder"))
                Cool::open((is_video ? path : Cool::File::without_file_name(path)).string().c_str());
        },
    });
}

void notification_after_export_failure()
{
    ImGui::Notify::add({
        .type    = ImGui::Notify::ToastType::Error,
        .title   = "Export Failed",
        .content = "Maybe you are not allowed to save files in this folder?",
    });
}

void notification_after_export_interrupted()
{
    ImGui::Notify::add({
        .type  = ImGui::Notify::ToastType::Warning,
        .title = "Export Cancelled",
    });
}

} // namespace Cool::ExporterU
