#include "ExporterU.h"
#include <Cool/File/File.h>
#include <Cool/Image/SaveImage.h>
#include <Cool/Log/ToUser.h>
#include <open/open.hpp>
#include "Cool/ImGui/ImGuiExtras.h"
#include "Cool/Task/Task.hpp"
#include "Cool/Task/TaskManager.hpp"
#include "ImGuiNotify/ImGuiNotify.hpp"
#include "no_sleep/no_sleep.hpp"

namespace Cool::ExporterU {

namespace {
class Task_SaveImage : public Task {
public:
    Task_SaveImage(std::filesystem::path const& file_path, img::Image image)
        : _file_path{file_path}
        , _image{std::move(image)}
    {
        File::mark_file_path_unavailable(file_path); // The file will not be created immediately, but we must know that it is already taken so that we don't try to create another image with the same path
    }

    void do_work() override
    {
        auto progress = std::atomic<float>{0.f};

        auto const notification_id = ImGuiNotify::send({
            .type                 = ImGuiNotify::Type::Info,
            .title                = fmt::format("Exporting Image \"{}\"", Cool::File::file_name(_file_path)),
            .custom_imgui_content = [cancel = &_cancel, progress = &progress]() { // It's okay to capture by pointer, we will change the notification and remove this lambda before exiting the do_work() function
                // ImGuiExtras::disabled_if(cancel->load(), "Canceled", [&]() {
                ImGuiExtras::progress_bar(progress->load());
                if (ImGui::Button("Cancel"))
                    cancel->store(true);
                // });
            },
            .duration = std::nullopt,
        });

        auto const result = ImageU::save(_file_path, _image, {.cancel = &_cancel, .progress = &progress});
        ImGuiNotify::change(
            notification_id, _cancel.load()       ? notification_after_image_export_canceled()
                             : result.has_value() ? notification_after_export_success(_file_path, false)
                                                  : notification_after_export_failure(result.error(), false)
        );
    }
    void cancel() override { _cancel.store(true); }
    auto needs_user_confirmation_to_cancel_when_closing_app() const -> bool override { return true; }

private:
    std::filesystem::path _file_path;
    img::Image            _image;

    std::atomic<bool> _cancel{false};

    no_sleep::Scoped _disable_sleep{COOL_APP_NAME, COOL_APP_NAME " is exporting an image", no_sleep::Mode::ScreenCanTurnOffButKeepComputing};
};
} // namespace

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

auto notification_after_export_success(std::filesystem::path const& path, bool is_video) -> ImGuiNotify::Notification
{
    return {
        .type                 = ImGuiNotify::Type::Success,
        .title                = is_video ? "Video Export Success" : "Image Export Success",
        .custom_imgui_content = [path, is_video]() {
            if (is_video)
            {
                if (ImGui::Button(fmt::format("Open \"{}\" folder", Cool::File::file_name(path)).c_str()))
                    open_folder_in_explorer(path);
            }
            else
            {
                if (ImGui::Button(fmt::format("Open \"{}\" in file explorer", Cool::File::file_name(path)).c_str()))
                    open_focused_in_explorer(path);
            }
        },
    };
}

auto notification_after_export_failure(std::string const& error_message, bool is_video) -> ImGuiNotify::Notification
{
    return {
        .type    = ImGuiNotify::Type::Error,
        .title   = is_video ? "Video Export Failed" : "Image Export Failed",
        .content = error_message,
    };
}

auto notification_after_image_export_canceled() -> ImGuiNotify::Notification
{
    return {
        .type  = ImGuiNotify::Type::Warning,
        .title = "Image Export Canceled",
    };
}

auto notification_after_video_export_canceled(std::filesystem::path const& path) -> ImGuiNotify::Notification
{
    return {
        .type                 = ImGuiNotify::Type::Warning,
        .title                = "Video Export Canceled",
        .custom_imgui_content = [path]() {
            if (ImGui::Button(fmt::format("Open \"{}\" folder", Cool::File::file_name(path)).c_str()))
                Cool::open_folder_in_explorer(path);
        },
    };
}

} // namespace Cool::ExporterU
