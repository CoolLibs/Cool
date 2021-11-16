#pragma once

#include <Cool/Path/Path.h>
#include <filesystem>
#if defined(DEBUG)
#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>
#endif
#include <Cool/Core/hide_console_in_release.h>

namespace Cool {

struct InitConfig {
    /// Pattern used by all the Log and Log::ToUser functions
    /// To learn how to configure it: https://github.com/gabime/spdlog/wiki/3.-Custom-formatting#pattern-flags
    std::string_view log_pattern = "%^[%e] %n: %v%$";

    /// Describes which (if any) tests you want to run
    std::function<void()> run_the_tests = []() {
#if defined(DEBUG)
        doctest::Context{}.run();
#endif
    };

    /// Sets the paths to the root directory and to Cool/res
    std::function<void()> set_paths = []() {
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
    };
};

} // namespace Cool
