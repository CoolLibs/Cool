#pragma once

namespace Cool {

/**
 * @brief An average that gets built over time as you push values into it. Useful to measure your average frame time for example
 *
 * @tparam T The type of the the values and the average. Should be a floating point type like float or double.
 */
template<typename T>
class Averager {
public:
    Averager()
    {
    }

    void push(T value)
    {
        _last[idx] = value;
        idx        = (idx + 1) % 5;
    }

    operator T()
    {
        return (_last[0] + _last[1] + _last[2] + _last[3] + _last[4]) / 5.f;
    }

    void clear()
    {
    }

private:
    std::array<T, 5> _last{};
    size_t           idx{0};
};

} // namespace Cool