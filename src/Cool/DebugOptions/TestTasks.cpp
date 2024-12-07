#include "TestTasks.hpp"
#include <imgui.h>
#include <memory>
#include "Cool/Log/ToUser.h"
#include "Cool/MultiThread/TaskManager.hpp"

namespace Cool {

namespace {

class Task_Test1 : public Task {
public:
    void do_work() override
    {
        Cool::Log::ToUser::info(fmt::format("Task {}", id), "Started");
        volatile int i = 0;
        for (int _ = 0; _ < 1000000000; ++_)
            i++;
        Cool::Log::ToUser::info(fmt::format("Task {}", id), "Stopped");
    }

private:
    auto next_id() -> int&
    {
        static int instance{0};
        return instance;
    }
    int id{next_id()++};
};

} // namespace

void TestTasks::imgui()
{
    ImGui::TextUnformatted(fmt::format("Waiting: {}", task_manager().tasks_waiting_count()).c_str());
    ImGui::TextUnformatted(fmt::format("Processing: {}", task_manager().tasks_processing_count()).c_str());
    if (ImGui::Button("Task 1"))
    {
        task_manager().add(std::make_shared<Task_Test1>());
    }
}

} // namespace Cool
