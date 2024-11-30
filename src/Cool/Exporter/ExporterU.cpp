#include "ExporterU.h"
#include <Cool/File/File.h>
#include <Cool/Image/SaveImage.h>
#include <Cool/Log/ToUser.h>
#include <open/open.hpp>
#include "ImGuiNotify/ImGuiNotify.hpp"
#include "no_sleep/no_sleep.hpp"

namespace Cool::ExporterU {

auto export_image(img::Size size, Time time, Time delta_time, Polaroid const& polaroid, std::filesystem::path const& file_path, std::function<void(std::filesystem::path const& exported_image_path)> const& on_image_exported) -> bool
{
    no_sleep::Scoped disable_sleep{COOL_APP_NAME, COOL_APP_NAME " is exporting an image", no_sleep::Mode::ScreenCanTurnOffButKeepComputing};
    polaroid.render(size, time, delta_time);
    bool const success = ImageU::save_png(file_path, polaroid.texture().download_pixels());
    if (success)
    {
        on_image_exported(file_path);
        notification_after_export_success(file_path, false);
    }
    else
    {
        notification_after_export_failure();
    }
    return success;
}

void notification_after_export_success(std::filesystem::path const& path, bool is_video)
{
    ImGuiNotify::send({
        .type                 = ImGuiNotify::Type::Success,
        .title                = "Export Success",
        .custom_imgui_content = [=]() {
            ImGui::TextUnformatted(Cool::File::file_name(path).string().c_str());
            if (ImGui::Button("Open folder"))
                Cool::open(is_video ? path : Cool::File::without_file_name(path));
        },
    });
}

void notification_after_export_failure()
{
    ImGuiNotify::send({
        .type    = ImGuiNotify::Type::Error,
        .title   = "Export Failed",
        .content = "Maybe you are not allowed to save files in this folder?",
    });
}

void notification_after_export_interrupted()
{
    ImGuiNotify::send({
        .type  = ImGuiNotify::Type::Warning,
        .title = "Export Cancelled",
    });
}

} // namespace Cool::ExporterU
