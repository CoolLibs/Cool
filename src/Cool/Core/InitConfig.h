#pragma once

#include <Cool/Core/hide_console_in_release.h>
#include <Cool/Path/Path.h>
#include <exe_path/exe_path.h>
#include <filesystem>

namespace Cool {

inline auto compute_root_path() -> std::filesystem::path
{
#if DEBUG
    // When launching from an IDE (which almost always means we are in Debug mode)
    // the initial current_path will be set to bin/Debug
    // but I prefer to have it at the root of the project, so that the assets we load
    // are the ones at the root of the project, not the ones that have been copied to the bin folders.
    // This means that if for example we do hot reloading of shaders
    // we can modify the source assets (at the root), instead of the ones that have been copied.
    // This is important because only the ones at the root are tracked in Git.
    return COOL_DEBUG_ONLY__CMAKE_SOURCE_DIR;
#else
    return exe_path::dir();
#endif
}

struct InitConfig {
    std::filesystem::path root_path            = compute_root_path();
    std::filesystem::path cool_res_path        = compute_root_path() / COOL_PATH / "res";
    std::filesystem::path default_texture_path = "";
};

} // namespace Cool
