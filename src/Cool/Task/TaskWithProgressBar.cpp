#include "TaskWithProgressBar.hpp"
#include "Cool/ImGui/ImGuiExtras.h"
#include "Cool/Task/TaskManager.hpp"

namespace Cool {

TaskWithProgressBar::TaskWithProgressBar()
    : Task{reg::generate_uuid() /* give a unique id to this task, so that we can cancel it */}
{}

void TaskWithProgressBar::on_submit()
{
    _notification_id = ImGuiNotify::send({
        .type                 = ImGuiNotify::Type::Info,
        .title                = name(),
        .custom_imgui_content = [&]() { // We can capture This be reference because the Task is stored in a shared_ptr, and on_cleanup we will change the Notification so that it doesn't have to reference the task anymore
            ImGuiExtras::disabled_if(_data.cancel.load(), "", [&]() {
                ImGui::TextUnformatted("Waiting for other tasks to finish before we can start this one");
                if (ImGui::Button("Cancel"))
                    task_manager().cancel_all(owner_id());
            });
        },
        .duration    = std::nullopt,
        .is_closable = false,
    });
}

void TaskWithProgressBar::just_before_work_starts()
{
    ImGuiNotify::change(
        _notification_id,
        {
            .type                 = ImGuiNotify::Type::Info,
            .title                = name(),
            .custom_imgui_content = [&]() { // We can capture This be reference because the Task is stored in a shared_ptr, and on_cleanup we will change the Notification so that it doesn't have to reference the task anymore
                ImGuiExtras::disabled_if(_data.cancel.load(), "", [&]() {
                    ImGuiExtras::progress_bar(_data.progress.load());
                    if (ImGui::Button("Cancel"))
                        task_manager().cancel_all(owner_id());
                });
            },
            .duration    = std::nullopt,
            .is_closable = false,
        }
    );
}

auto TaskWithProgressBar::notification_after_do_work_completes() const -> ImGuiNotify::Notification
{
    return {
        .type    = ImGuiNotify::Type::Success,
        .title   = name(),
        .content = "Completed",
    };
}

void TaskWithProgressBar::on_cleanup(bool has_been_canceled)
{
    if (has_been_canceled)
        ImGuiNotify::close_immediately(_notification_id);
    else
        ImGuiNotify::change(_notification_id, notification_after_do_work_completes());
}

} // namespace Cool