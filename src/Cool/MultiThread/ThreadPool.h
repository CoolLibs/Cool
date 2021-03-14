#pragma once

namespace Cool {

class ThreadPool {
public:
	ThreadPool(size_t nb_threads);
	~ThreadPool();

	bool has_idle_threads();
	bool has_finished_all_jobs();
	void start();
	void stop();
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