#include "TestTasks.hpp"
#include <imgui.h>
#include <ImGuiNotify/ImGuiNotify.hpp>
#include <chrono>
#include <memory>
#include <optional>
#include <reg/src/generate_uuid.hpp>
#include <string>
#include "Cool/ImGui/ImGuiExtras.h"
#include "Cool/Log/ToUser.h"
#include "Cool/Task/TaskManager.hpp"

namespace Cool {

namespace {

auto next_id() -> int&
{
    static int instance{0};
    return instance;
}

class Task_LongProcess : public Task {
public:
    explicit Task_LongProcess(int count_max)
        : Task{reg::generate_uuid() /* give a unique id to this task, so that we can cancel it */}
        , _count_max{count_max}
    {}

    void on_submit() override
    {
        Cool::Log::ToUser::info(fmt::format("Task {}", _id), "Submitted");

        _notification_id = ImGuiNotify::send({
            .type                 = ImGuiNotify::Type::Info,
            .title                = fmt::format("Task {}", _id),
            .custom_imgui_content = [data = _data, task_id = owner_id()]() { // The lambda needs to capture everything by copy
                ImGuiExtras::disabled_if(data->cancel.load(), "", [&]() {
                    ImGui::TextUnformatted("Waiting for other tasks to finish before we can start this one");
                    if (ImGui::Button("Cancel"))
                        task_manager().cancel_all(task_id);
                });
            },
            .duration    = std::nullopt,
            .is_closable = false,
        });
    }

    void on_cleanup(bool has_been_canceled) override
    {
        if (has_been_canceled)
        {
            Cool::Log::ToUser::info(fmt::format("Task {}", _id), "Canceled");
            ImGuiNotify::close_immediately(_notification_id);
        }
        else
        {
            Cool::Log::ToUser::info(fmt::format("Task {}", _id), "Finished");
            ImGuiNotify::change(
                _notification_id,
                {
                    .type    = ImGuiNotify::Type::Success,
                    .title   = fmt::format("Task {}", _id),
                    .content = "Success",
                }
            );
        }
    }

    void do_work() override
    {
        Cool::Log::ToUser::info(fmt::format("Task {}", _id), "Started");
        ImGuiNotify::change(
            _notification_id,
            {
                .type                 = ImGuiNotify::Type::Info,
                .title                = fmt::format("Task {}", _id),
                .custom_imgui_content = [data = _data, task_id = owner_id()]() { // The lambda needs to capture everything by copy
                    ImGuiExtras::disabled_if(data->cancel.load(), "", [&]() {
                        ImGuiExtras::progress_bar(data->progress.load());
                        if (ImGui::Button("Cancel"))
                            task_manager().cancel_all(task_id);
                    });
                },
                .duration    = std::nullopt,
                .is_closable = false,
            }
        );

        int count{0};
        for (int _ = 0; _ < _count_max; ++_)
        {
            if (_data->cancel.load())
                return;
            count++;
            _data->progress.store(static_cast<float>(count) / static_cast<float>(_count_max));
        }
    }

    void cancel() override { _data->cancel.store(true); }
    auto needs_user_confirmation_to_cancel_when_closing_app() const -> bool override { return true; }
    auto name() const -> std::string override { return "Test Task: Long Process"; }

private:
    int                         _id{next_id()++};
    int                         _count_max;
    ImGuiNotify::NotificationId _notification_id{};

    struct DataSharedWithNotification {
        std::atomic<bool>  cancel{false}; // Needs to be an atomic because it will be used on several threads (by the Task and by the Notification)
        std::atomic<float> progress{0.f};
    };
    std::shared_ptr<DataSharedWithNotification> _data{std::make_shared<DataSharedWithNotification>()}; // Needs to be a shared_ptr because the Notification will need to keep it alive after this task is done
};

class Task_SayHello : public QuickTask {
public:
    explicit Task_SayHello(bool loop = false)
        : _loop{loop}
    {}

    void do_work() override
    {
        ImGuiNotify::send({
            .type    = _loop ? ImGuiNotify::Type::Warning : ImGuiNotify::Type::Success,
            .title   = fmt::format("Task {}", _id),
            .content = fmt::format("Hello from {} ago", std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - _start_time)),
        });
        if (_loop)
            task_manager().submit_small_task_to_run_in(2s, std::make_shared<Task_SayHello>(true));
    }

    void on_cleanup(bool has_been_canceled) override
    {
        Cool::Log::ToUser::info(fmt::format("Task Say Hello {}", _id), has_been_canceled ? "Canceled" : "Finished");
    }

    auto needs_user_confirmation_to_cancel_when_closing_app() const -> bool override { return false; }
    auto name() const -> std::string override { return "Test Task: Say Hello"; }

private:
    int  _id{next_id()++};
    bool _loop{};

    std::chrono::steady_clock::time_point _start_time{std::chrono::steady_clock::now()};
};

} // namespace

void TestTasks::imgui()
{
    ImGui::TextUnformatted(fmt::format("Waiting: {}", task_manager().tasks_waiting_count()).c_str());
    ImGui::TextUnformatted(fmt::format("Processing: {}", task_manager().tasks_processing_count()).c_str());
    ImGui::TextUnformatted(fmt::format("Small delayed tasks: {}", task_manager().small_delayed_tasks_count()).c_str());

    if (ImGui::Button("Long Task"))
        task_manager().submit(std::make_shared<Task_LongProcess>(1000000000));
    if (ImGui::Button("Medium Task"))
        task_manager().submit(std::make_shared<Task_LongProcess>(100000000));
    if (ImGui::Button("Short Task"))
        task_manager().submit(std::make_shared<Task_LongProcess>(10000000));
    if (ImGui::Button("Run in 2 seconds"))
        task_manager().submit_small_task_to_run_in(2s, std::make_shared<Task_SayHello>());
    if (ImGui::Button("Loop every 2 seconds"))
        task_manager().submit_small_task_to_run_in(0s, std::make_shared<Task_SayHello>(true));

    ImGui::NewLine();
    if (ImGui::Button("Cancel all tasks"))
        task_manager().cancel_all();
}

} // namespace Cool
