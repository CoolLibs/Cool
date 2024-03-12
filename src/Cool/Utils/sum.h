#pragma once
#include <numeric>

namespace Cool {

template<typename T>
auto sum(std::vector<T> const& v) -> T
{
    return std::accumulate(v.begin(), v.end(), T{0});
}

} // namespace Cool