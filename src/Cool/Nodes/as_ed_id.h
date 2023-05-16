#pragma once

namespace Cool {

inline auto as_ed_id(reg::AnyId const& id)
{
    return (static_cast<uint64_t>(id.underlying_uuid().as_bytes()[0]) << 0)
           + (static_cast<uint64_t>(id.underlying_uuid().as_bytes()[1]) << 8)
           + (static_cast<uint64_t>(id.underlying_uuid().as_bytes()[2]) << 16)
           + (static_cast<uint64_t>(id.underlying_uuid().as_bytes()[3]) << 24)
           + (static_cast<uint64_t>(id.underlying_uuid().as_bytes()[4]) << 32)
           + (static_cast<uint64_t>(id.underlying_uuid().as_bytes()[5]) << 40)
           + (static_cast<uint64_t>(id.underlying_uuid().as_bytes()[6]) << 48)
           + (static_cast<uint64_t>(id.underlying_uuid().as_bytes()[7]) << 56);
}

} // namespace Cool