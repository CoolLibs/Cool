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
	/// <returns>true iff some threads are idle and can accept a job</returns>
	bool has_idle_threads();

	/// <summary>
	/// 
	/// </summary>
	/// <returns>true iff all threads are idle</returns>
	bool has_finished_all_jobs();

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

private:
	size_t _nb_threads;
	std::vector<std::thread> _threads;
	std::condition_variable _condition;
	std::deque<std::function<void()>> _queue;
	std::mutex _queue_mutex;
	bool _running = false;
};

} // namespace Cool