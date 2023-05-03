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

inline auto italic() -> ImFont*&
{
    static ImFont* font{}; // NOLINT(*-avoid-non-const-global-variables)
    return font;
}

inline auto heading_1() -> ImFont*&
{
    static ImFont* font{}; // NOLINT(*-avoid-non-const-global-variables)
    return font;
}

inline auto heading_2() -> ImFont*&
{
    static ImFont* font{}; // NOLINT(*-avoid-non-const-global-variables)
    return font;
}

inline auto heading_3() -> ImFont*&
{
    static ImFont* font{}; // NOLINT(*-avoid-non-const-global-variables)
    return font;
}

inline auto window_title() -> ImFont*&
{
    static ImFont* font{}; // NOLINT(*-avoid-non-const-global-variables)
    return font;
}

inline auto monospace() -> ImFont*&
{
    static ImFont* font{}; // NOLINT(*-avoid-non-const-global-variables)
    return font;
}

} // namespace Cool::Font