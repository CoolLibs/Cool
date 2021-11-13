#include "DefaultInit.h"

#if defined(DEBUG)
#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>
#endif
#include <Cool/Log/Log.h>
#include <Cool/Path/Path.h>
#include <filesystem>

namespace CoolDefault {

void init()
{
    Cool::Log::initialize();
#if defined(DEBUG)
    // Run the tests !
    doctest::Context{}.run();
#endif

#if defined(DEBUG)
    // When launching from an IDE (which almost always means we are in Debug mode)
    // the initial current_path will be set to bin/Debug
    // but I prefer to have it at the root of the project, so that the assets we load
    // are the ones at the root of the project, not the ones that have been copied to the bin folders.
    // This means that if for example we do hot reloading of shaders
    // we can modify the source assets (at the root), instead of the ones that have been copied.
    // This is important because only the ones at the root are tracked in Git.
    std::filesystem::current_path(COOL_DEBUG_ONLY__CMAKE_SOURCE_DIR);
#endif
    Cool::Path::initialize_root(std::filesystem::current_path().string());
    Cool::Path::initialize_cool_res(std::filesystem::current_path().string() + "/" + COOL_PATH + "/res");
}

} // namespace CoolDefault