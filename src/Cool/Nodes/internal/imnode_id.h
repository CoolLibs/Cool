#pragma once
#include <reg/reg.hpp>

namespace Cool {

template<typename T>
auto imnode_id(reg::Id<T> const& id) -> int
{
    return imnode_id(id.underlying_uuid());
}

inline auto imnode_id(uuids::uuid const& id) -> int
{
    const auto bytes = id.as_bytes();

    return (std::to_integer<int>(bytes[0]) << 0) |
           (std::to_integer<int>(bytes[1]) << 8) |
           (std::to_integer<int>(bytes[2]) << 16) |
           (std::to_integer<int>(bytes[3]) << 24);
}

} // namespace Cool