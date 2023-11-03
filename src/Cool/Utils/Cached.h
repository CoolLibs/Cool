#pragma once

namespace Cool {

template<typename T>
class Cached {
public:
    /// Returns the last computed value,
    /// or computes a new one using `compute_value` if `invalidate_cache()` has been called after the previous call to `get_value()`/
    auto get_value(std::function<T()> const& compute_value) -> T const&
    {
        if (!_is_valid)
        {
            _value    = compute_value();
            _is_valid = true;
        }
        return _value;
    }

    void invalidate_cache() { _is_valid = false; }

private:
    T    _value{};
    bool _is_valid{false};
};

} // namespace Cool