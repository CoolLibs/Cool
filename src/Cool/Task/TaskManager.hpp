#pragma once
#include <chrono>
#include <condition_variable>
#include <reg/src/AnyId.hpp>
#include <thread>
#include "Cool/Utils/Delay.hpp"
#include "Task.hpp"

namespace Cool {

// TODO(Tasks) need to block till all tasks are finished before shutting down the app (eg test what happens when closing the window during a video export)
// and also, tell tasks to cancel asap (tasks decide what they do with that info, maybe they are doing something like saving an image and we want to wait till it completes)
// and also the tasks that have not started yet, they must tell us if they need to run or can be skipped
class TaskManager {
public:
    TaskManager();
    ~TaskManager()                                 = default;
    TaskManager(TaskManager const&)                = delete;
    TaskManager& operator=(TaskManager const&)     = delete;
    TaskManager(TaskManager&&) noexcept            = delete;
    TaskManager& operator=(TaskManager&&) noexcept = delete;

    void submit(std::shared_ptr<Task> const& task);
    void submit_in(std::chrono::milliseconds delay, std::shared_ptr<Task> const& task);
    /// This task will be run on the main thread (to make sure the delay is respected precisely, it avoids having all worker threads blocked by huge tasks and not being able to submit this task precisely when the timer runs out)
    /// So it must run very quickly, in order to not block the main thread
    void run_small_task_in(std::chrono::milliseconds delay, std::shared_ptr<Task> const& task);
    /// Wraps the lambda in a task and calls `run_small_task_in()`
    /// ⚠ The lambda must capture everything by copy, it will be stored
    void run_small_lambda_in(std::chrono::milliseconds delay, std::function<void()> const& lambda);
    void cancel_all_tasks(reg::AnyId const& owner_id);

    auto tasks_waiting_count(reg::AnyId const& owner_id) const -> size_t;
    auto tasks_processing_count(reg::AnyId const& owner_id) const -> size_t;
    auto small_delayed_tasks_count(reg::AnyId const& owner_id) const -> size_t;
    auto threads_count() const -> size_t { return _threads.size(); }

private:
    friend class TestTasks;
    auto tasks_waiting_count() const -> size_t;
    auto tasks_processing_count() const -> size_t;
    auto small_delayed_tasks_count() const -> size_t;

private:
    friend class AppManager;
    void update_on_main_thread();
    auto has_tasks_that_need_user_confirmation_before_killing() const -> bool;
    void shut_down();

private:
    void thread_update_loop();
    void cancel_all_tasks();

private:
    std::deque<std::shared_ptr<Task>> _tasks_waiting;
    std::list<std::shared_ptr<Task>>  _tasks_processing;
    mutable std::shared_mutex         _tasks_mutex;

    std::vector<std::thread>    _threads;
    std::condition_variable_any _wake_up_thread;
    std::condition_variable_any _wait_for_threads_to_finish;
    std::atomic<bool>           _is_shutting_down{false};

    struct TaskAndDelay {
        std::shared_ptr<Task> task;
        Delay                 delay;
    };
    std::list<TaskAndDelay>   _small_tasks_with_delay;
    mutable std::shared_mutex _small_tasks_with_delay_mutex;
};

inline auto task_manager() -> TaskManager&
{
    static auto instance = TaskManager{};
    return instance;
}

} // namespace Cool