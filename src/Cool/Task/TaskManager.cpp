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

static void execute_task(Task& task)
{
    task.do_work();
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
            execute_task(*task);
        }
        _tasks_processing_count.fetch_sub(1);
    }
}

void TaskManager::run_small_task_in(std::chrono::milliseconds delay, std::shared_ptr<Task> const& task)
{
    auto lock = std::unique_lock{_small_tasks_with_delay_mutex};
    _small_tasks_with_delay.push_back({task, Delay{delay}});
}

void TaskManager::update_on_main_thread()
{
    auto lock = std::unique_lock{_small_tasks_with_delay_mutex};
    for (auto it = _small_tasks_with_delay.begin(); it != _small_tasks_with_delay.end();)
    {
        if (!it->delay.has_expired())
        {
            ++it;
            continue;
        }
        lock.unlock(); // Unlock in case the task wants to add some other tasks (this is safe because we use a std::list, so even if we push_back in the list, we can then resume iteration here)
        execute_task(*it->task);
        lock.lock();
        it = _small_tasks_with_delay.erase(it);
    }
}

auto TaskManager::tasks_waiting_count() const -> size_t
{
    auto lock = std::shared_lock{_tasks_queue_mutex};
    return _tasks_queue.size();
}

auto TaskManager::small_delayed_tasks_count() const -> size_t
{
    auto lock = std::shared_lock{_small_tasks_with_delay_mutex};
    return _small_tasks_with_delay.size();
}

} // namespace Cool