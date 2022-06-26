#pragma once

namespace Cool {

enum class BuildMode {
    Debug,
    Release,
    Any,
    None,
};

[[nodiscard]] inline constexpr bool is_building_as(BuildMode build_mode)
{
    switch (build_mode) {
    case BuildMode::Debug:
#if DEBUG
        return true;
#else
        return false;
#endif
    case BuildMode::Release:
#if DEBUG
        return false;
#else
        return true;
#endif
    case BuildMode::Any:
        return true;
    case BuildMode::None:
        return false;
    default: {
        Log::error("[is_building_as] Unknown enum value");
        return false;
    }
    }
}

} // namespace Cool
