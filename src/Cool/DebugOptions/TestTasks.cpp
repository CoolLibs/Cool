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
#include "Cool/Task/TaskCoroutine.hpp"
#include "Cool/Task/TaskManager.hpp"
#include "Cool/Task/TaskWithProgressBar.hpp"

namespace Cool {

namespace {

auto next_id() -> int&
{
    static int instance{0};
    return instance;
}

class Task_LongProcess : public TaskWithProgressBar {
public:
    explicit Task_LongProcess(uint64_t count_max)
        : _count_max{count_max}
    {}

    auto name() const -> std::string override { return fmt::format("Long Process {}", _id); }

private:
    auto execute() -> TaskCoroutine override
    {
        auto const start_time = std::chrono::steady_clock::now();

        volatile uint64_t count{0};
        // auto start = std::chrono::steady_clock::now();
        for (uint64_t _ = 0; _ < 200; ++_)
        {
            for (uint64_t i = 0; i < _count_max / 200; ++i)
                count++;

            if (cancel_requested())
                co_return;
            set_progress(static_cast<float>(_) / static_cast<float>(200));
            // if (_ % 1000 == 0)
            // if (std::chrono::steady_clock::now() - start > 1000ms)
            {
                co_await SuspendTask{};
                // start = std::chrono::steady_clock::now();
            }
        }
        Cool::Log::ToUser::info("dsf", fmt::format("Took {}", std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start_time)));
    }

    auto needs_user_confirmation_to_cancel_when_closing_app() const -> bool override { return true; }

private:
    int      _id{next_id()++};
    uint64_t _count_max;
};

class Task_SayHello : public Task {
public:
    explicit Task_SayHello(bool loop = false)
        : _loop{loop}
    {}

    auto name() const -> std::string override { return fmt::format("Say Hello {}", _id); }

private:
    auto execute() -> TaskCoroutine override
    {
        ImGuiNotify::send({
            .type    = _loop ? ImGuiNotify::Type::Warning : ImGuiNotify::Type::Success,
            .title   = name(),
            .content = fmt::format("Hello from {} ago", std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - _start_time)),
        });
        if (_loop)
            task_manager().submit(after(2s), std::make_shared<Task_SayHello>(true));
        co_return;
    }

    auto is_quick_task() const -> bool override { return true; }
    void cancel() override {}
    auto needs_user_confirmation_to_cancel_when_closing_app() const -> bool override { return false; }

private:
    int  _id{next_id()++};
    bool _loop{};

    std::chrono::steady_clock::time_point _start_time{std::chrono::steady_clock::now()};
};

} // namespace

void TestTasks::imgui()
{
    ImGui::TextUnformatted(fmt::format("Waiting: {}", task_manager().num_tasks_waiting()).c_str());
    ImGui::TextUnformatted(fmt::format("Processing: {}", task_manager().num_tasks_processing()).c_str());

    if (ImGui::Button("Long Task"))
        task_manager().submit(std::make_shared<Task_LongProcess>(10000000000));
    if (ImGui::Button("Medium Task"))
        task_manager().submit(std::make_shared<Task_LongProcess>(1000000000));
    if (ImGui::Button("Short Task"))
        task_manager().submit(std::make_shared<Task_LongProcess>(100000000));
    if (ImGui::Button("Run in 2 seconds"))
        task_manager().submit(after(2s), std::make_shared<Task_SayHello>());
    if (ImGui::Button("Loop every 2 seconds"))
        task_manager().submit(std::make_shared<Task_SayHello>(true));

    ImGui::NewLine();
    if (ImGui::Button("Cancel all tasks"))
        task_manager().cancel_all();
}

} // namespace Cool
