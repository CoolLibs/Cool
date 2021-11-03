#pragma once

namespace std {
inline std::ostream& operator<<(std::ostream& os, const std::vector<std::string>& v)
{
    for (const auto& x : v) {
        os << x << " ";
    }
    return os;
}
} // namespace std