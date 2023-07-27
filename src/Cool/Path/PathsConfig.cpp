#include "PathsConfig.h"
#include "Cool/Path/Path.h"
#include "Cool/Path/PathsConfig.h"
#include "Path.h"
#include "exe_path/exe_path.h"

namespace Cool {

PathsConfig::PathsConfig(std::string app_name)
    : _app_name{std::move(app_name)}
{
}

auto PathsConfig::root() const -> std::filesystem::path
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

auto PathsConfig::cool_res() const -> std::filesystem::path
{
    return Cool::Path::root() / COOL_PATH / "res";
}

auto PathsConfig::user_data() const -> std::filesystem::path
{
#if DEBUG                                    // Make sure you don't mess up your installed app while developing.
    return Cool::Path::root() / "User data"; // And also, make sure it won't be shared with other repositories of the same project, that could break serialization at any time: we don't want them affecting each other.
#else
    return exe_path::user_data() / _app_name;
#endif
}

auto PathsConfig::default_user_data() const -> std::filesystem::path
{
    return Cool::Path::root() / "User data Default";
}

auto PathsConfig::default_texture() const -> std::filesystem::path
{
    return "";
}

} // namespace Cool