#pragma once
#include <chrono>

namespace Cool {

class Chrono {
public:
    /// Returns true iff more than `duration` has elapsed since the last call to `elapsed_more_than()`, or to the creation of the class when this is called for the first time.
    template<typename Duration>
    auto elapsed_more_than(Duration const& duration) -> bool
    {
        auto const now = std::chrono::steady_clock::now();
        bool const res = now - _last_time > duration;
        _last_time     = now;
        return res;
    }

private:
    std::chrono::steady_clock::time_point _last_time{std::chrono::steady_clock::now()};
};

} // namespace Cool