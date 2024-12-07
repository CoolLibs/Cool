#include "TaskManager.hpp"

namespace Cool {

TaskManager::TaskManager()
{
    auto const size = std::thread::hardware_concurrency() > 1 ? std::thread::hardware_concurrency() - 1 : 1;
    _threads.reserve(size);
    for (size_t i = 0; i < size; ++i)
        _threads.emplace_back([&]() { thread_update_loop(); });
}

TaskManager::~TaskManager()
{
    _is_shutting_down.store(true);
    { // Wait for the jobs queue to be empty
        auto lock = std::unique_lock{_tasks_queue_mutex};
    }
    _wake_up_thread.notify_all(); // Wake up all threads and let them realize that _is_shutting_down == true
    for (std::thread& thread : _threads)
        thread.join();
}

void TaskManager::submit(std::shared_ptr<Task> const& task)
{
    {
        auto lock = std::unique_lock{_tasks_queue_mutex};
        _tasks_queue.push_back(task);
    }
    _wake_up_thread.notify_one();
}

void TaskManager::thread_update_loop()
{
    while (true)
    {
        {
            auto task = std::shared_ptr<Task>{};
            { // Grab a task from the queue
                auto lock = std::unique_lock{_tasks_queue_mutex};
                _wake_up_thread.wait(lock, [&] { return !_tasks_queue.empty() || _is_shutting_down.load(); });
                if (_is_shutting_down.load())
                    break; // TODO(Tasks) shouldn't we actually wait until all tasks have been processed?
                task = std::move(_tasks_queue.front());
                _tasks_queue.pop_front();
                _tasks_processing_count.fetch_add(1);
            }
            task->do_work();
        }
        _tasks_processing_count.fetch_sub(1);
    }
}

auto TaskManager::tasks_waiting_count() const -> size_t
{
    auto lock = std::shared_lock{_tasks_queue_mutex};
    return _tasks_queue.size();
}

} // namespace Cool