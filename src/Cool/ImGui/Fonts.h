#pragma once

namespace Cool::Font {

inline auto regular() -> ImFont*&
{
    static ImFont* font{}; // NOLINT(*-avoid-non-const-global-variables)
    return font;
}

inline auto bold() -> ImFont*&
{
    static ImFont* font{}; // NOLINT(*-avoid-non-const-global-variables)
    return font;
}

inline auto console() -> ImFont*&
{
    static ImFont* font{}; // NOLINT(*-avoid-non-const-global-variables)
    return font;
}

} // namespace Cool::Font