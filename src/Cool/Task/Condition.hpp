#pragma once
#include <chrono>
#include "Cool/Utils/Delay.hpp"

namespace Cool {

/// Returns true iff the task is ready to be submitted
using Condition = std::function<bool()>;

class Condition_Delay {
public:
    explicit Condition_Delay(std::chrono::milliseconds delay)
        : _delay{delay}
    {}

    auto operator()() const -> bool
    {
        return _delay.has_expired();
    }

private:
    Delay _delay;
};

} // namespace Cool