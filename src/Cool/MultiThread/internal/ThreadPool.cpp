#include "../ThreadPool.h"

namespace Cool {

ThreadPool::ThreadPool(size_t nb_threads)
	: _nb_threads(nb_threads)
{}

ThreadPool::~ThreadPool() {
	if (_running) {
		stop();
	}
}

bool ThreadPool::has_idle_threads() {
	return _queue.size() < _nb_threads;
}

bool ThreadPool::has_finished_all_jobs() {
	return _queue.size() == 0;
}

void ThreadPool::start() {
	_threads.reserve(_nb_threads);
	for (size_t i = 0; i < _nb_threads; ++i) {
		_threads.emplace_back([this]() { check_for_jobs(); });
	}
	_running = true;
}

void ThreadPool::stop() {
	while (!has_finished_all_jobs()) {} // Wait for the jobs queue to be empty
	_running = false;
	_condition.notify_all(); // Wake up all threads and let them realize that _running == false
	for (std::thread& thread : _threads) {
		thread.join();
	}
	_threads.clear();
}

void ThreadPool::push_job(std::function<void()> job) {
	{
		std::unique_lock<std::mutex> lock(_queue_mutex);
		_queue.emplace_back(std::move(job));
	}
	_condition.notify_one();
}

void ThreadPool::check_for_jobs() {
	std::function<void()> job;
	while (true) {
		{
			std::unique_lock<std::mutex> lock(_queue_mutex);
			_condition.wait(lock, [this] {return !_queue.empty() || !_running; });
			if (!_running)
				break;
			job = std::move(_queue.front());
			_queue.pop_front();
		}
		job();
	}
}

} // namespace Cool