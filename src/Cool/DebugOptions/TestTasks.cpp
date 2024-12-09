#include "TestTasks.hpp"
#include <imgui.h>
#include <ImGuiNotify/ImGuiNotify.hpp>
#include <chrono>
#include <memory>
#include <optional>
#include <string>
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
    {}

    void do_work() override
    {
        auto count = std::make_shared<std::atomic<int>>(0); // Needs to be a shared_ptr because the Notification will need to keep it alive after this task is done
                                                            // Needs to be an atomic because it will be used on several threads (by the Task and by the Notification)

        auto const notification_id = ImGuiNotify::send({
            .type                 = ImGuiNotify::Type::Info,
            .title                = fmt::format("Task {}", _id),
            .custom_imgui_content = [count, count_max = static_cast<float>(_count_max)]() { // The lambda needs to capture everything by copy
                ImGui::ProgressBar(static_cast<float>(count->load()) / count_max);
            },
            .duration = std::nullopt,
        });

        Cool::Log::ToUser::info(fmt::format("Task {}", _id), "Started");

        for (int _ = 0; _ < _count_max; ++_)
        {
            if (_cancel.load())
            {
                Cool::Log::ToUser::info(fmt::format("Task {}", _id), "Canceled");
                ImGuiNotify::close(notification_id, 1s);
                return;
            }
            count->fetch_add(1);
        }

        Cool::Log::ToUser::info(fmt::format("Task {}", _id), "Stopped");
        ImGuiNotify::close(notification_id, 1s);
    }

    void cancel() override
    {
        _cancel.store(true);
    }

    auto needs_user_confirmation_to_cancel_when_closing_app() const -> bool override
    {
        return true;
    }

private:
    int _id{next_id()++};

    std::atomic<bool> _cancel{false};
    int               _count_max;
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
