#pragma once
#include <chrono>

namespace Cool {

auto time_to_wait() -> std::chrono::steady_clock::duration;

}