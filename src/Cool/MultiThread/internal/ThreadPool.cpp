#include "../ThreadPool.h"

namespace Cool {

ThreadPool::ThreadPool()
	: ThreadPool(std::thread::hardware_concurrency() > 1 ? std::thread::hardware_concurrency() - 1 : 1)
{}

ThreadPool::ThreadPool(size_t nb_threads)
	: _nb_threads(nb_threads)
{
	Log::Info("Using {} threads in the thread pool", nb_threads);
}

ThreadPool::~ThreadPool() {
	if (_running) {
		stop();
	}
}

void ThreadPool::wait_for_available_thread() {
	assert(_running && "You must call start() before working with the thread pool.");
	std::unique_lock<std::mutex> lock(_jobs_queue_mutex);
	_condition_to_check_queue_size_is_small_enough.wait(lock, [this] { return _jobs_queue.size() < _nb_threads; });
}

void ThreadPool::wait_for_all_jobs_to_finish() {
	std::unique_lock<std::mutex> lock(_jobs_queue_mutex);
	_condition_to_check_queue_size_is_small_enough.wait(lock, [this] { return _jobs_queue.empty(); });
}

void ThreadPool::start() {
	assert(!_running && "You already called start.");
	_threads.reserve(_nb_threads);
	for (size_t i = 0; i < _nb_threads; ++i) {
		_threads.emplace_back([this]() { check_for_jobs(); });
	}
	_running = true;
}

void ThreadPool::stop() {
	assert(_running && "You already called stop or you forgot to call start.");
	// Wait for the jobs queue to be empty
	wait_for_all_jobs_to_finish();
	_running = false;
	_condition_to_pop_from_queue.notify_all(); // Wake up all threads and let them realize that _running == false
	for (std::thread& thread : _threads) {
		thread.join();
	}
	_threads.clear();
}

void ThreadPool::push_job(std::function<void()> job) {
	assert(_running && "You must call start() before working with the thread pool.");
	{
		std::unique_lock<std::mutex> lock(_jobs_queue_mutex);
		_jobs_queue.emplace_back(std::move(job));
	}
	_condition_to_pop_from_queue.notify_one();
}

void ThreadPool::check_for_jobs() {
	std::function<void()> job;
	while (true) {
		{
			std::unique_lock<std::mutex> lock(_jobs_queue_mutex);
			_condition_to_pop_from_queue.wait(lock, [this] {return !_jobs_queue.empty() || !_running; });
			if (!_running)
				break;
			job = std::move(_jobs_queue.front());
			_jobs_queue.pop_front();
		}
		job();
		_condition_to_check_queue_size_is_small_enough.notify_all();
	}
}

} // namespace Cool