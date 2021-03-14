#pragma once

#include <thread>
#include <deque>

namespace Cool {

class ThreadPool {
public:
	ThreadPool();
	~ThreadPool() = default;
};

} // namespace Cool