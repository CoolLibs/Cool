#pragma once

namespace Cool::Font {

inline auto console() -> ImFont*&
{
    static ImFont* font{}; // NOLINT(*-avoid-non-const-global-variables)
    return font;
}

} // namespace Cool::Font