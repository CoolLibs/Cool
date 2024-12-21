#include "TaskManager.hpp"
#include <algorithm>
#include <memory>

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
    cancel_all_tasks();
    { // Wait for threads that are processing a task to finish
        auto lock = std::unique_lock{_tasks_mutex};
        _wait_for_threads_to_finish.wait(lock, [&] { return _tasks_processing.empty(); });
    }
    // Wake up all threads that were not processing a task and let them realize that _is_shutting_down == true
    _wake_up_thread.notify_all();
    // Close all threads
    for (std::thread& thread : _threads)
        thread.join();
}

void TaskManager::submit(std::shared_ptr<Task> const& task)
{
    if (_is_shutting_down.load())
        return;
    {
        auto lock = std::unique_lock{_tasks_mutex};
        _tasks_waiting.push_back(task);
    }
    _wake_up_thread.notify_one();
}

class Task_SubmitTask : public Task {
public:
    explicit Task_SubmitTask(std::shared_ptr<Task> const& task)
        : _task{task}
    {}

    void do_work() override { task_manager().submit(_task); }
    auto needs_user_confirmation_to_cancel_when_closing_app() const -> bool override { return _task->needs_user_confirmation_to_cancel_when_closing_app(); }
    void cancel() override { _task->cancel(); }

private:
    std::shared_ptr<Task> _task;
};

void TaskManager::submit_in(std::chrono::milliseconds delay, std::shared_ptr<Task> const& task)
{
    run_small_task_in(delay, std::make_shared<Task_SubmitTask>(task));
}

void TaskManager::run_small_task_in(std::chrono::milliseconds delay, std::shared_ptr<Task> const& task)
{
    if (_is_shutting_down.load())
        return;
    auto lock = std::unique_lock{_small_tasks_with_delay_mutex};
    _small_tasks_with_delay.push_back({task, Delay{delay}});
}

namespace {
class Task_Lambda : public Task {
public:
    explicit Task_Lambda(std::function<void()> const& lambda)
        : _lambda{lambda}
    {}

    void do_work() override { _lambda(); }
    void cancel() override {}
    auto needs_user_confirmation_to_cancel_when_closing_app() const -> bool override { return false; }

private:
    std::function<void()> _lambda;
};
} // namespace

void TaskManager::run_small_lambda_in(std::chrono::milliseconds delay, std::function<void()> const& lambda)
{
    run_small_task_in(delay, std::make_shared<Task_Lambda>(lambda));
}

void TaskManager::cancel_all_tasks()
{
    {
        auto lock = std::unique_lock{_small_tasks_with_delay_mutex};
        for (auto& task : _small_tasks_with_delay)
            task.task->cancel();
        _small_tasks_with_delay.clear();
    }
    {
        auto lock = std::unique_lock{_tasks_mutex};
        for (auto& task : _tasks_waiting)
            task->cancel();
        _tasks_waiting.clear();
        for (auto& task : _tasks_processing)
            task->cancel();
    }
}

void TaskManager::cancel_all_tasks(reg::AnyId const& owner_id)
{
    {
        auto lock = std::unique_lock{_small_tasks_with_delay_mutex};
        std::erase_if(_small_tasks_with_delay, [&](TaskAndDelay const& task) {
            return task.task->owner_id() == owner_id;
        });
    }
    {
        auto lock = std::unique_lock{_tasks_mutex};
        std::erase_if(_tasks_waiting, [&](std::shared_ptr<Task> const& task) {
            return task->owner_id() == owner_id;
        });
        for (auto& task : _tasks_processing)
        {
            if (task->owner_id() == owner_id)
                task->cancel();
        }
    }
}

static void execute_task(Task& task)
{
    task.do_work();
}

void TaskManager::thread_update_loop()
{
    while (true)
    {
        auto task = std::shared_ptr<Task>{};
        { // Grab a task from the queue
            auto lock = std::unique_lock{_tasks_mutex};
            _wake_up_thread.wait(lock, [&] { return !_tasks_waiting.empty() || _is_shutting_down.load(); });
            if (_is_shutting_down.load())
                break;
            task = std::move(_tasks_waiting.front());
            _tasks_waiting.pop_front();
            _tasks_processing.push_back(task);
        }
        execute_task(*task);
        {
            auto lock = std::unique_lock{_tasks_mutex};
            std::erase_if(_tasks_processing, [&](std::shared_ptr<Task> const& t) { return t.get() == task.get(); });
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
    auto lock = std::shared_lock{_tasks_mutex};
    return _tasks_waiting.size();
}
auto TaskManager::tasks_processing_count() const -> size_t
{
    auto lock = std::shared_lock{_tasks_mutex};
    return _tasks_processing.size();
}
auto TaskManager::small_delayed_tasks_count() const -> size_t
{
    auto lock = std::shared_lock{_small_tasks_with_delay_mutex};
    return _small_tasks_with_delay.size();
}
auto TaskManager::tasks_waiting_count(reg::AnyId const& owner_id) const -> size_t
{
    auto lock = std::shared_lock{_tasks_mutex};
    return std::count_if(_tasks_waiting.begin(), _tasks_waiting.end(), [&](std::shared_ptr<Task> const& task) {
        return task->owner_id() == owner_id;
    });
}
auto TaskManager::tasks_processing_count(reg::AnyId const& owner_id) const -> size_t
{
    auto lock = std::shared_lock{_tasks_mutex};
    return std::count_if(_tasks_processing.begin(), _tasks_processing.end(), [&](std::shared_ptr<Task> const& task) {
        return task->owner_id() == owner_id;
    });
}
auto TaskManager::small_delayed_tasks_count(reg::AnyId const& owner_id) const -> size_t
{
    auto lock = std::shared_lock{_small_tasks_with_delay_mutex};
    return std::count_if(_small_tasks_with_delay.begin(), _small_tasks_with_delay.end(), [&](TaskAndDelay const& task) {
        return task.task->owner_id() == owner_id;
    });
}

auto TaskManager::has_tasks_that_need_user_confirmation_before_killing() const -> bool
{
    auto lock1 = std::shared_lock{_tasks_mutex};
    auto lock2 = std::shared_lock{_small_tasks_with_delay_mutex};
    return std::any_of(_tasks_processing.begin(), _tasks_processing.end(), [](std::shared_ptr<Task> const& task) {
               return task->needs_user_confirmation_to_cancel_when_closing_app();
           })
           || std::any_of(_tasks_waiting.begin(), _tasks_waiting.end(), [](std::shared_ptr<Task> const& task) {
                  return task->needs_user_confirmation_to_cancel_when_closing_app();
              })
           || std::any_of(_small_tasks_with_delay.begin(), _small_tasks_with_delay.end(), [](TaskAndDelay const& task) {
                  return task.task->needs_user_confirmation_to_cancel_when_closing_app();
              });
}

} // namespace Cool