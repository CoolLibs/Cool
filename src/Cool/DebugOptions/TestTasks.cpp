#include "TestTasks.hpp"
#include <imgui.h>
#include <ImGuiNotify/ImGuiNotify.hpp>
#include <chrono>
#include <memory>
#include <optional>
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
        : _count_max{count_max}
    {
        _notification_id = ImGuiNotify::send({
            .type                 = ImGuiNotify::Type::Info,
            .title                = fmt::format("Task {}", _id),
            .custom_imgui_content = [data = _data]() { // The lambda needs to capture everything by copy
                ImGuiExtras::disabled_if(data->cancel.load(), "", [&]() {
                    ImGuiExtras::progress_bar(data->progress.load());
                    if (ImGui::Button("Cancel"))
                        data->cancel.store(true);
                });
            },
            .is_closable = false,
            .duration    = std::nullopt,
        });
    }

    void do_work() override
    {
        Cool::Log::ToUser::info(fmt::format("Task {}", _id), "Started");

        int count{0};
        for (int _ = 0; _ < _count_max; ++_)
        {
            if (_data->cancel.load())
                return;
            count++;
            _data->progress.store(static_cast<float>(count) / static_cast<float>(_count_max));
        }

        Cool::Log::ToUser::info(fmt::format("Task {}", _id), "Stopped");
        ImGuiNotify::close_after_small_delay(_notification_id);
    }

    void cancel() override
    {
        _data->cancel.store(true);
        Cool::Log::ToUser::info(fmt::format("Task {}", _id), "Canceled");
        ImGuiNotify::close_immediately(_notification_id);
    }
    auto needs_user_confirmation_to_cancel_when_closing_app() const -> bool override { return true; }

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

class Task_SayHello : public Task {
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
            task_manager().run_small_task_in(2s, std::make_shared<Task_SayHello>(true));
    }

    void cancel() override
    {
        // Nothing to do, this task runs instantly anyways
    }

    auto needs_user_confirmation_to_cancel_when_closing_app() const -> bool override
    {
        return false;
    }

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
        task_manager().run_small_task_in(2s, std::make_shared<Task_SayHello>());
    if (ImGui::Button("Loop every 2 seconds"))
        task_manager().run_small_task_in(0s, std::make_shared<Task_SayHello>(true));

    ImGui::NewLine();
    if (ImGui::Button("Cancel all tasks"))
        task_manager().cancel_all_tasks();
}

} // namespace Cool
