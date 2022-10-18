#pragma once

namespace Cool {

/**
 * @brief An average that gets built over time as you push values into it. Useful to measure your average frame time for example
 *
 * @tparam Float The type of the the values and the average. Should be a floating point type like float or double.
 */
template<typename Float>
class Averager {
public:
    explicit Averager(size_t buffer_size = 10)
        : _buffer(buffer_size)
    {}

    Averager(Averager const&)            = delete;
    Averager& operator=(Averager const&) = delete;

    void push(Float value)
    {
        _buffer[_idx] = value;
        _idx          = (_idx + 1) % buffer_size();
        _current_size = std::min(_current_size + 1, buffer_size());
    }

    operator Float() const
    {
        if (_current_size == 0)
            return Float{1};

        Float      average{0};
        const auto _current_size_as_float = static_cast<Float>(_current_size);
        for (size_t i = 0; i < _current_size; ++i)
        {
            average += _buffer[(_idx + buffer_size() - i) % buffer_size()] / _current_size_as_float;
        }
        return average;
    }

    void clear()
    {
        _current_size = 0;
    }

private:
    auto buffer_size() const -> size_t { return _buffer.size(); }

private:
    std::vector<Float> _buffer;
    size_t             _idx{0};
    size_t             _current_size{0};
};

} // namespace Cool