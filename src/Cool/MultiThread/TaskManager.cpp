#include "TaskManager.hpp"

namespace Cool {

TaskManager::TaskManager()
{
    auto const size = std::thread::hardware_concurrency() > 1 ? std::thread::hardware_concurrency() - 1 : 1;
    _threads.reserve(size);
    for (size_t i = 0; i < size; ++i)
        _threads.emplace_back([&]() { thread_update_loop(); });
}

void TaskManager::add(std::shared_ptr<Task> const& task)
{
    {
        auto lock = std::unique_lock{_tasks_queue_mutex};
        _tasks_queue.push_back(task);
    }
    _wake_up_thread.notify_one();
}

void TaskManager::thread_update_loop()
{
    auto task = std::shared_ptr<Task>{};
    while (true)
    {
        { // Grab a task from the queue
            auto lock = std::unique_lock{_tasks_queue_mutex};
            _wake_up_thread.wait(lock, [&] { return !_tasks_queue.empty() || _is_shutting_down.load(); });
            if (_is_shutting_down.load())
                break; // TODO(Tasks) shouldn't we actually wait until all tasks have been processed?
            task = std::move(_tasks_queue.front());
            _tasks_queue.pop_front();
        }
        task->do_work();
    }
}

auto TaskManager::tasks_waiting_count() const -> size_t
{
    auto lock = std::shared_lock{_tasks_queue_mutex};
    return _tasks_queue.size();
}

auto TaskManager::tasks_processing_count() const -> size_t
{
    return 0; // TODO(Tasks)
}

} // namespace Cool