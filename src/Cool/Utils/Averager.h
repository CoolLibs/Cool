#pragma once

namespace Cool {

/// An average that gets built over time as you push values into it. Useful to measure your average frame time for example.
/// Float is the type of the the values and the average. Should be a floating point type like float or double.
template<std::floating_point Float>
class Averager {
public:
    Averager()
    {
        clear();
    }

    void clear()
    {
        _average = Float{0};
        _N       = 0;
    }

    void push(Float value)
    {
        _N++;
        const auto N = static_cast<Float>(_N);
        _average     = (_average * (N - 1) + value) / N;
    }

    operator Float() const { return get(); }

    auto get() const -> Float { return _average; }

private:
    Float        _average;
    unsigned int _N;
};

} // namespace Cool