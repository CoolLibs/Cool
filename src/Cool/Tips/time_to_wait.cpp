#include "time_to_wait.h"

namespace Cool {

auto time_to_wait() -> std::chrono::steady_clock::duration
{
    return 10min;
}

} // namespace Cool