namespace Cool {

template<typename Job>
ThreadPool<Job>::ThreadPool()
    : ThreadPool(std::thread::hardware_concurrency() > 1 ? std::thread::hardware_concurrency() - 1 : 1)
{
}

template<typename Job>
ThreadPool<Job>::ThreadPool(size_t nb_threads)
    : _nb_threads(nb_threads)
{
    Log::info("[ThreadPool::ThreadPool] Using {} threads in the thread pool", nb_threads);
}

template<typename Job>
ThreadPool<Job>::~ThreadPool()
{
    if (_running) {
        stop();
    }
}



template<typename Job>
void ThreadPool<Job>::wait_for_all_jobs_to_finish()
{
    std::unique_lock<std::mutex> lock(_jobs_queue_mutex);
    _condition_to_check_queue_size_is_small_enough.wait(lock, [this] { return _jobs_queue.empty(); });
}

template<typename Job>
void ThreadPool<Job>::start()
{
    assert(!_running && "You already called start.");
    _threads.reserve(_nb_threads);
    for (size_t i = 0; i < _nb_threads; ++i) {
        _threads.emplace_back([this]() { check_for_jobs(); });
    }
    _running = true;
}

template<typename Job>
void ThreadPool<Job>::stop()
{
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

template<typename Job>
void ThreadPool<Job>::push_job(Job&& job)
{
    assert(_running && "You must call start() before working with the thread pool.");
    {
        std::unique_lock<std::mutex> lock(_jobs_queue_mutex);
        _jobs_queue.push_back(std::move(job));
    }
}

template<typename Job>
void ThreadPool<Job>::check_for_jobs()
{
    Job job;
    while (true) {
        {
            std::unique_lock<std::mutex> lock(_jobs_queue_mutex);
            _condition_to_pop_from_queue.wait(lock, [this] { return !_jobs_queue.empty() || !_running; });
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