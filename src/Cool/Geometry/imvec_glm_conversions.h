#pragma once
#include <imgui.h>

namespace Cool {

inline auto as_imvec(glm::vec2 v) -> ImVec2
{
    return {v.x, v.y};
}
inline auto as_glm(ImVec2 v) -> glm::vec2
{
    return {v.x, v.y};
}
inline auto as_imvec(glm::vec4 const& v) -> ImVec4
{
    return {v.x, v.y, v.z, v.w};
}
inline auto as_glm(ImVec4 const& v) -> glm::vec4
{
    return {v.x, v.y, v.z, v.w};
}

} // namespace Cool