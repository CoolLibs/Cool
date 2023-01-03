#pragma once

#include <reg/reg.hpp>

namespace IMNODES_NAMESPACE {

using ID = reg::AnyId;
static const ID INVALID_ID{};

inline void PushID(ID const& id)
{
    ImGui::PushID(uuids::to_string(id.underlying_uuid()).c_str());
}

inline std::string IDToString(ID const& id)
{
    return uuids::to_string(id.underlying_uuid());
}

inline ID IDFromString(std::string const& str)
{
    return *uuids::uuid::from_string(str);
}

inline bool operator<(ID const& id1, ID const& id2)
{
    return id1.underlying_uuid() < id2.underlying_uuid();
}

inline bool operator>(ID const& id1, ID const& id2)
{
    return !(id1.underlying_uuid() < id2.underlying_uuid()) && id1 != id2;
}

} // namespace IMNODES_NAMESPACE