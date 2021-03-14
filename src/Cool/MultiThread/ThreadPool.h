#pragma once

namespace Cool {

/// <summary>
/// Manages a given number of threads and give them jobs
/// </summary>
class ThreadPool {
public:	
	/// <summary>
	/// Creates a thread pool using the maximum number of concurrent threads supported by the machine
	/// </summary>
	ThreadPool();

	/// <summary>
	/// 
	/// </summary>
	/// <param name="nb_threads">Number of threads to create in the pool</param>
	ThreadPool(size_t nb_threads);
	~ThreadPool();

	/// <summary>
	/// 
	/// </summary>
	/// <returns>The number of threads in the pool</returns>
	inline size_t size() { return _nb_threads; }

	/// <summary>
	/// Blocks the calling thread until a thread is available to accept a job.
	/// Using this function is not mandatory but makes sure that the queue of jobs waiting for a worker thread doesn't get too big.
	/// </summary>
	void wait_for_available_thread();

	/// <summary>
	/// Starts the pool : creates the threads
	/// </summary>
	void start();

	/// <summary>
	/// Stops the pool : finishes all jobs and then destroys the threads
	/// </summary>
	void stop();

	/// <summary>
	/// Add a job to the queue.
	/// </summary>
	/// <param name="job"></param>
	void push_job(std::function<void()> job);

private:
	void check_for_jobs();
	void wait_for_all_jobs_to_finish();

private:
	size_t _nb_threads;
	std::vector<std::thread> _threads;
	std::condition_variable _condition_to_pop_from_queue;
	std::condition_variable _condition_to_check_queue_size_is_small_enough;
	std::deque<std::function<void()>> _queue;
	std::mutex _queue_mutex;
	bool _running = false;
};

} // namespace Cool