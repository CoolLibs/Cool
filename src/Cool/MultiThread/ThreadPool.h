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
    inline size_t size() { return _nb_threads; }

    bool has_available_worker() { return _jobs_queue.size() < size(); }

    /**
     * @brief Starts the pool : creates the threads
     *
     */
    void start();

    /**
     * @brief Stops the pool : finishes all jobs and then destroys the threads
     *
     */
    void stop();

    /**
     * @brief Adds a job to the queue.
     *
     * @param job Any function pointer / lambda with signature void -> void
     */
    void push_job(Job&& job);

private:
    void check_for_jobs();
    void wait_for_all_jobs_to_finish();

private:
    size_t                   _nb_threads;
    std::vector<std::thread> _threads;
    std::condition_variable  _condition_to_pop_from_queue;
    std::condition_variable  _condition_to_check_queue_size_is_small_enough;
    std::deque<Job>          _jobs_queue;
    std::mutex               _jobs_queue_mutex;
    bool                     _running = false;
};

} // namespace Cool

#include "ThreadPool.tpp"

// Waiting for xcode to implement jthread

// #pragma once

// namespace Cool {

// /**
//  * @brief Manages a given number of threads and gives them jobs
//  *
//  */
// template<typename Job>
// class ThreadPool {
// public:
//     /**
//      * @brief Creates a thread pool using the maximum number of concurrent threads supported by the machine
//      *
//      */
//     ThreadPool();

//     /**
//      * @brief
//      *
//      * @param nb_threads Number of threads to create in the pool
//      */
//     ThreadPool(size_t nb_threads);

//     ~ThreadPool();

//     /**
//      * @brief
//      *
//      * @return The number of threads in the pool
//      */
//     auto size() { return _threads.size(); }

//     bool has_available_worker() { return _jobs_queue.size() < size(); }

//     /**
//      * @brief Adds a job to the queue.
//      *
//      * @param job Any function pointer / lambda with signature void -> void
//      */
//     void push_job(Job&& job);

// private:
//     void worker_thread_loop(std::stop_token stop_token);

// private:
//     std::vector<std::jthread> _threads;
//     std::condition_variable   _wake_up_thread;
//     std::deque<Job>           _jobs_queue;
//     std::mutex                _jobs_queue_mutex;
// };

// } // namespace Cool

// #include "ThreadPool.tpp"