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
        clear();
    }

    void push(T value)
    {
        _N++;
        _average = (_average * (_N - 1) + value) / _N;
    }

    operator T()
    {
        return _average;
    }

    void clear()
    {
        _average = 0;
        _N       = 0;
    }

private:
    T            _average;
    unsigned int _N;
};

} // namespace Cool