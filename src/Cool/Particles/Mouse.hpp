#pragma once

namespace Cool {

inline auto mouse_pos() -> glm::vec2&
{
    static auto inst = glm::vec2{};
    return inst;
}
inline auto mouse_pressed() -> bool&
{
    static auto inst = bool{false};
    return inst;
}

} // namespace Cool