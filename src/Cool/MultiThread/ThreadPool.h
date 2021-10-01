#pragma once

namespace Cool {

/**
 * @brief Manages a given number of threads and give them jobs
 * 
 */
template<typename Job>
class ThreadPool {
public:
    /**
	 * @brief Creates a thread pool using the maximum number of concurrent threads supported by the machine
	 * 
	 */
    ThreadPool();

    /**
	 * @brief
	 * 
	 * @param nb_threads Number of threads to create in the pool
	 */
    ThreadPool(size_t nb_threads);

    ~ThreadPool();

    /**
	 * @brief 
	 * 
	 * @return The number of threads in the pool
	 */
    auto size() { return _threads.size(); }

    bool has_available_worker() { return _jobs_queue.size() < size(); }

    /**
	 * @brief Adds a job to the queue.
	 * 
	 * @param job Any function pointer / lambda with signature void -> void
	 */
    void push_job(Job&& job);

private:
    void check_for_jobs(std::stop_token stop_token);
    void wait_for_all_jobs_to_finish();

private:
    std::vector<std::jthread> _threads;
    std::condition_variable   _wake_up_thread;
    std::deque<Job>           _jobs_queue;
    std::mutex                _jobs_queue_mutex;
};

} // namespace Cool

#include "ThreadPool.tpp"