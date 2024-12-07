#pragma once
#include <condition_variable>
#include <thread>
#include "Task.hpp"

namespace Cool {

// TODO(Tasks) need to block till all tasks are finished before shutting down the app
// and also, tell tasks to cancel asap (tasks decide what they do with that info, maybe they are doing something like saving an image and we want to wait till it completes)
// and also the tasks that have not started yet, they must tell us if they need to run or can be skipped
class TaskManager {
public:
    TaskManager();

    void add(std::shared_ptr<Task> const& task);

    auto tasks_waiting_count() const -> size_t;
    auto tasks_processing_count() const -> size_t;

private:
    void thread_update_loop();
    void wait_for_all_tasks_to_finish();

private:
    std::deque<std::shared_ptr<Task>> _tasks_queue;
    mutable std::shared_mutex         _tasks_queue_mutex;

    std::vector<std::thread>    _threads;
    std::condition_variable_any _wake_up_thread;
    std::atomic<bool>           _is_shutting_down{false};
};

inline auto task_manager() -> TaskManager&
{
    static auto instance = TaskManager{};
    return instance;
}

} // namespace Cool