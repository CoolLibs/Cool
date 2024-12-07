#pragma once
#include <chrono>
#include <condition_variable>
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
    ~TaskManager();
    TaskManager(TaskManager const&)                = delete;
    TaskManager& operator=(TaskManager const&)     = delete;
    TaskManager(TaskManager&&) noexcept            = delete;
    TaskManager& operator=(TaskManager&&) noexcept = delete;

    void submit(std::shared_ptr<Task> const& task);
    /// These tasks will be run on the main thread (to make sure the delay is respected precisely, it avoids having all worker threads blocked by huge tasks and not being able to submit this task precisely when the timer runs out)
    /// So they must run very quickly, in order to not block the main thread
    void run_small_task_in(std::chrono::milliseconds delay, std::shared_ptr<Task> const& task);

    auto tasks_waiting_count() const -> size_t;
    auto tasks_processing_count() const -> size_t { return _tasks_processing_count.load(); }
    auto threads_count() const -> size_t { return _threads.size(); }
    auto small_delayed_tasks_count() const -> size_t;

private:
    friend class AppManager;
    void update_on_main_thread();

private:
    void thread_update_loop();
    void wait_for_all_tasks_to_finish();

private:
    std::deque<std::shared_ptr<Task>> _tasks_queue;
    mutable std::shared_mutex         _tasks_queue_mutex;

    std::vector<std::thread>    _threads;
    std::condition_variable_any _wake_up_thread;
    std::atomic<bool>           _is_shutting_down{false};

    std::atomic<size_t> _tasks_processing_count{0};

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