#include "TaskManager.hpp"
#include <algorithm>
#include <chrono>
#include <memory>
#include "TaskCoroutine.hpp"

namespace Cool {

TaskManager::TaskManager()
{
    auto const size = std::thread::hardware_concurrency() > 1 ? std::thread::hardware_concurrency() - 1 : 1;
    _threads.reserve(size);
    for (size_t i = 0; i < size; ++i)
        _threads.emplace_back([&]() { thread_update_loop(); });
}

void TaskManager::shut_down()
{
    // Make sure we don't accept tasks anymore, tell threads to stop asap
    _is_shutting_down.store(true);
    // Remove waiting tasks, tell processing tasks to finish asap
    cancel_all();
    { // Wait for threads that are processing a task to finish
        auto lock = std::unique_lock{_tasks_processing_mutex};
        _wait_for_threads_to_finish.wait(lock, [&] { return _tasks_processing.empty(); });
    }
    // Wake up all threads that were not processing a task and let them realize that _is_shutting_down == true
    _wake_up_thread.notify_all();
    // Close all threads
    for (std::thread& thread : _threads)
        thread.join();
}

void TaskManager::execute_task(Task& task)
{
    task.just_before_execution_starts();
    task.execute();
    if (!task.has_been_canceled())
        task._completion.store(Task::Completion::FinishedExecuting);
    task.cleanup(task.has_been_canceled());
}

void TaskManager::cancel_task_that_is_waiting(Task& task)
{
    // Task has not started execute(), so there is no need to call cancel()
    task._completion.store(Task::Completion::Canceled);
    task.cleanup(true /*has_been_canceled*/);
}

void TaskManager::cancel_task_that_is_executing(Task& task)
{
    task.cancel();
    task._completion.store(Task::Completion::Canceled);
    // cleanup() will be called by the thread that is running the task once execute() finishes
}

void TaskManager::thread_update_loop()
{
    while (true)
    {
        auto task = std::optional<TaskAndCoroutine>{};
        { // Grab a task from the queue
            auto lock = std::unique_lock{_tasks_processing_mutex};
            _wake_up_thread.wait(lock, [&] { return !_tasks_processing.empty() || _is_shutting_down.load(); });
            if (_tasks_processing.empty())
                continue;
            if (_is_shutting_down.load())
                break;
            task = std::move(_tasks_processing.front());
            _tasks_processing.pop_front();
            _nb_tasks_processed_by_thread.fetch_add(1);
        }

        // auto const start = std::chrono::steady_clock::now();
        // while (!task->coroutine.has_finished() && std::chrono::steady_clock::now() - start < 1000ms)
        // while (!task->coroutine.has_finished())
        task->coroutine.do_some_work();
        _nb_tasks_processed_by_thread.fetch_add(-1);
        if (task->coroutine.has_finished())
        {
            task->task->cleanup(false /*has_been_canceled*/);
        }
        else
        {
            auto lock = std::unique_lock{_tasks_processing_mutex};
            _tasks_processing.emplace_back(std::move(*task));
            _wake_up_thread.notify_one();
        }

        if (_is_shutting_down.load())
        {
            _wait_for_threads_to_finish.notify_one();
            break;
        }
    }
}

void TaskManager::update_on_main_thread()
{
    auto tasks_to_start  = std::vector<std::shared_ptr<Task>>{}; // We don't submit the tasks immediately in the loop, because they might be executed immediately, and if they want to submit other tasks this would conflict with the lock
    auto tasks_to_cancel = std::vector<std::shared_ptr<Task>>{}; // We don't cancel the tasks immediately in the loop, because the custom code in their cleanup() might conflict with the lock

    {
        auto lock = std::unique_lock{_tasks_waiting_mutex};
        for (auto it = _tasks_waiting.begin(); it != _tasks_waiting.end();)
        {
            if (!it->condition || it->condition->wants_to_execute())
            {
                tasks_to_start.emplace_back(it->task);
                it = _tasks_waiting.erase(it);
            }
            else if (it->condition->wants_to_cancel())
            {
                tasks_to_cancel.emplace_back(it->task);
                it = _tasks_waiting.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    {
        auto lock = std::unique_lock{_tasks_processing_mutex};
        for (auto const& task : tasks_to_start)
        {
            task->just_before_execution_starts();
            _tasks_processing.emplace_back(task, task->execute());
            _wake_up_thread.notify_one();
        }
    }
    for (auto const& task : tasks_to_cancel)
        cancel_task_that_is_waiting(*task);
}

void TaskManager::submit(std::shared_ptr<Task> const& task)
{
    submit(nullptr /*condition*/, task);
}

void TaskManager::submit(std::shared_ptr<WaitToExecuteTask> const& condition, std::shared_ptr<Task> const& task)
{
    if (_is_shutting_down.load())
        return;

    task->on_submit();

    auto lock = std::unique_lock{_tasks_waiting_mutex};
    _tasks_waiting.emplace_back(task, condition);
}

void TaskManager::cancel_if(std::function<bool(Task const&)> const& predicate)
{
    // {
    //     auto lock = std::unique_lock{_tasks_with_condition_mutex};

    //     for (TaskAndCondition& task : _tasks_with_condition)
    //     {
    //         if (!predicate(*task.task))
    //             continue;

    //         cancel_task_that_is_waiting(*task.task);
    //         task.task = nullptr; // Mark them as nullptr so that we can erase_if afterwards without having to check the predicate again
    //     }
    //     std::erase_if(_tasks_with_condition, [](TaskAndCondition const& task) { return task.task == nullptr; });
    // }
    // {
    //     auto lock = std::unique_lock{_tasks_mutex};

    //     for (std::shared_ptr<Task>& task : _tasks_waiting)
    //     {
    //         if (!predicate(*task))
    //             continue;

    //         cancel_task_that_is_waiting(*task);
    //         task = nullptr; // Mark them as nullptr so that we can erase_if afterwards without having to check the predicate again
    //     };
    //     std::erase_if(_tasks_waiting, [](std::shared_ptr<Task> const& task) { return task == nullptr; });

    //     // Cancel the tasks that are already in progress
    //     for (auto& task : _tasks_processing)
    //     {
    //         if (!predicate(*task))
    //             continue;
    //         cancel_task_that_is_executing(*task);
    //     }
    // }
}

void TaskManager::cancel_all()
{
    cancel_if([&](Task const&) {
        return true;
    });
}

void TaskManager::cancel_all(reg::AnyId const& owner_id)
{
    cancel_if([&](Task const& task) {
        return task.owner_id() == owner_id;
    });
}

auto TaskManager::num_tasks_waiting() const -> size_t
{
    auto lock = std::shared_lock{_tasks_waiting_mutex};
    return _tasks_waiting.size();
}

auto TaskManager::num_tasks_processing() const -> size_t
{
    auto lock = std::shared_lock{_tasks_processing_mutex};
    return _tasks_processing.size() + _nb_tasks_processed_by_thread.load();
}

auto TaskManager::num_tasks_waiting_for_thread(reg::AnyId const& owner_id) const -> size_t
{
    return 1;
    // auto lock = std::shared_lock{_tasks_mutex};
    // return std::count_if(_tasks_waiting.begin(), _tasks_waiting.end(), [&](std::shared_ptr<Task> const& task) {
    //     return task->owner_id() == owner_id;
    // });
}

auto TaskManager::num_tasks_waiting_for_condition(reg::AnyId const& owner_id) const -> size_t
{
    return 1;
    // auto lock = std::shared_lock{_tasks_with_condition_mutex};
    // return std::count_if(_tasks_with_condition.begin(), _tasks_with_condition.end(), [&](TaskAndCondition const& task) {
    //     return task.task->owner_id() == owner_id;
    // });
}

auto TaskManager::num_tasks_processing(reg::AnyId const& owner_id) const -> size_t
{
    return 1;
    // auto lock = std::shared_lock{_tasks_mutex};
    // return std::count_if(_tasks_processing.begin(), _tasks_processing.end(), [&](std::shared_ptr<Task> const& task) {
    //     return task->owner_id() == owner_id;
    // });
}

auto TaskManager::list_of_tasks_that_need_user_confirmation_before_killing() const -> std::string
{
    auto list_of_tasks = ""s;
    // auto const maybe_add_task = [&](Task const& task) {
    //     if (!task.needs_user_confirmation_to_cancel_when_closing_app())
    //         return;
    //     if (!list_of_tasks.empty())
    //         list_of_tasks += '\n';
    //     list_of_tasks += fmt::format(" - {}", task.name());
    // };

    // {
    //     auto lock1 = std::shared_lock{_tasks_mutex};
    //     auto lock2 = std::shared_lock{_tasks_with_condition_mutex};

    //     for (std::shared_ptr<Task> const& task : _tasks_processing)
    //         maybe_add_task(*task);
    //     for (std::shared_ptr<Task> const& task : _tasks_waiting)
    //         maybe_add_task(*task);
    //     for (TaskAndCondition const& task : _tasks_with_condition)
    //         maybe_add_task(*task.task);
    // }

    return list_of_tasks;
}

} // namespace Cool