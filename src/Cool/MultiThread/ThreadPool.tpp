// #include "../ThreadPool.h"

namespace Cool {

template<typename Job>
ThreadPool<Job>::ThreadPool()
    : ThreadPool(std::thread::hardware_concurrency() > 1 ? std::thread::hardware_concurrency() - 1 : 1)
{
}

template<typename Job>
ThreadPool<Job>::ThreadPool(size_t nb_threads)
{
    Log::info("[ThreadPool::ThreadPool] Using {} threads in the thread pool", nb_threads);
    _threads.reserve(nb_threads);
    for (size_t i = 0; i < nb_threads; ++i) {
        _threads.emplace_back([this](std::stop_token st) { check_for_jobs(st); });
    }
}

template<typename Job>
ThreadPool<Job>::~ThreadPool()
{
    for (auto& thread : _threads) {
        thread.request_stop();
    }
    _wake_up_thread.notify_all();
}

template<typename Job>
void ThreadPool<Job>::wait_for_available_thread()
{
    std::unique_lock<std::mutex> lock(_jobs_queue_mutex);
    _condition_to_check_queue_size_is_small_enough.wait(lock, [this] { return _jobs_queue.size() < size(); });
}

template<typename Job>
void ThreadPool<Job>::push_job(Job&& job)
{
    {
        std::unique_lock<std::mutex> lock(_jobs_queue_mutex);
        _jobs_queue.push_back(std::move(job));
    }
    _wake_up_thread.notify_one();
}

template<typename Job>
void ThreadPool<Job>::check_for_jobs(std::stop_token stop_token)
{
    Job job;
    while (!stop_token.stop_requested()) {
        {
            std::unique_lock<std::mutex> lock(_jobs_queue_mutex);
            _wake_up_thread.wait(lock, [this, stop_token] { return !_jobs_queue.empty() || stop_token.stop_requested(); });
            if (stop_token.stop_requested()) {
                break;
            }
            job = std::move(_jobs_queue.front());
            _jobs_queue.pop_front();
        }
        job();
        _condition_to_check_queue_size_is_small_enough.notify_all();
    }
}

} // namespace Cool