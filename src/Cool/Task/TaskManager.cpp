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
    cancel_all();
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
        task->do_work();
        task->on_cleanup(task->_has_been_canceled_during_do_work.load());
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
        it->task->do_work();
        it->task->on_cleanup(false /*has_been_canceled*/);
        lock.lock();
        it = _small_tasks_with_delay.erase(it);
    }
}

void TaskManager::submit(std::shared_ptr<Task> const& task)
{
    if (_is_shutting_down.load())
        return;

    task->on_submit();

    if (task->is_quick_task())
    {
        task->do_work();
        task->on_cleanup(false /*has_been_canceled*/);
    }
    else
    {
        {
            auto lock = std::unique_lock{_tasks_mutex};
            _tasks_waiting.push_back(task);
        }
        _wake_up_thread.notify_one();
    }
}

void TaskManager::submit_small_task_to_run_in(std::chrono::milliseconds delay, std::shared_ptr<Task> const& task)
{
    if (_is_shutting_down.load())
        return;
    task->on_submit();
    {
        auto lock = std::unique_lock{_small_tasks_with_delay_mutex};
        _small_tasks_with_delay.push_back({task, Delay{delay}});
    }
}

void TaskManager::cancel_if(std::function<bool(Task const&)> const& predicate)
{
    {
        auto lock = std::unique_lock{_small_tasks_with_delay_mutex};

        for (TaskAndDelay& task : _small_tasks_with_delay)
        {
            if (!predicate(*task.task))
                continue;

            task.task->on_cleanup(true /*has_been_canceled*/); // Task has not started do_work(), so there is no need to call cancel()

            task.task = nullptr; // Mark them as nullptr so that we can erase_if afterwards without having to check the predicate again
        }
        std::erase_if(_small_tasks_with_delay, [](TaskAndDelay const& task) { return task.task == nullptr; });
    }
    {
        auto lock = std::unique_lock{_tasks_mutex};

        for (std::shared_ptr<Task>& task : _tasks_waiting)
        {
            if (!predicate(*task))
                continue;

            task->on_cleanup(true /*has_been_canceled*/); // Task has not started do_work(), so there is no need to call cancel()

            task = nullptr; // Mark them as nullptr so that we can erase_if afterwards without having to check the predicate again
        };
        std::erase_if(_tasks_waiting, [](std::shared_ptr<Task> const& task) { return task == nullptr; });

        // Cancel the tasks that are already in progress
        for (auto& task : _tasks_processing)
        {
            if (!predicate(*task))
                continue;
            task->cancel(); // on_cleanup() will be called by the thread that is running the task once do_work() finishes
            task->_has_been_canceled_during_do_work.store(true);
        }
    }
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

class Task_SubmitTask : public Task {
public:
    explicit Task_SubmitTask(std::shared_ptr<Task> const& task)
        : _task{task}
    {}

    void do_work() override { task_manager().submit(_task); } //, false /*don't call on_submit() again*/); }
    void on_submit() override { _task->on_submit(); }
    auto needs_user_confirmation_to_cancel_when_closing_app() const -> bool override { return _task->needs_user_confirmation_to_cancel_when_closing_app(); }
    auto name() const -> std::string override { return fmt::format("Waiting to submit: {}", _task->name()); }
    void cancel() override { _task->cancel(); }

private:
    std::shared_ptr<Task> _task;
};

void TaskManager::submit_in(std::chrono::milliseconds delay, std::shared_ptr<Task> const& task)
{
    submit_small_task_to_run_in(delay, std::make_shared<Task_SubmitTask>(task));
}

namespace {
class Task_Lambda : public Task {
public:
    explicit Task_Lambda(std::string name, std::function<void()> const& lambda)
        : _name(std::move(name))
        , _lambda{lambda}
    {}

    void do_work() override { _lambda(); }
    void cancel() override {}
    auto needs_user_confirmation_to_cancel_when_closing_app() const -> bool override { return false; }
    auto name() const -> std::string override { return _name; }

private:
    std::string           _name;
    std::function<void()> _lambda;
};
} // namespace

void TaskManager::submit_small_lambda_to_run_in(std::chrono::milliseconds delay, std::string name, std::function<void()> const& lambda)
{
    submit_small_task_to_run_in(delay, std::make_shared<Task_Lambda>(std::move(name), lambda));
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

auto TaskManager::list_of_tasks_that_need_user_confirmation_before_killing() const -> std::string
{
    auto       list_of_tasks  = ""s;
    auto const maybe_add_task = [&](Task const& task) {
        if (!task.needs_user_confirmation_to_cancel_when_closing_app())
            return;
        if (!list_of_tasks.empty())
            list_of_tasks += '\n';
        list_of_tasks += fmt::format(" - {}", task.name());
    };

    {
        auto lock1 = std::shared_lock{_tasks_mutex};
        auto lock2 = std::shared_lock{_small_tasks_with_delay_mutex};

        for (std::shared_ptr<Task> const& task : _tasks_processing)
            maybe_add_task(*task);
        for (std::shared_ptr<Task> const& task : _tasks_waiting)
            maybe_add_task(*task);
        for (TaskAndDelay const& task : _small_tasks_with_delay)
            maybe_add_task(*task.task);
    }

    return list_of_tasks;
}

} // namespace Cool