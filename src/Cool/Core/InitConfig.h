#pragma once

#include <Cool/Path/Path.h>
#include <exe_path/exe_path.h>
#include <filesystem>
#if DEBUG
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
#if DEBUG
        if (doctest::Context{}.run())
        {
            Cool::Log::error_without_breakpoint("Program closed because some tests failed.");
            std::exit(1);
        }
#endif
    };

    /// Sets the paths to the root directory and to Cool/res
    std::function<void()> set_paths = []() {
#if DEBUG
        // When launching from an IDE (which almost always means we are in Debug mode)
        // the initial current_path will be set to bin/Debug
        // but I prefer to have it at the root of the project, so that the assets we load
        // are the ones at the root of the project, not the ones that have been copied to the bin folders.
        // This means that if for example we do hot reloading of shaders
        // we can modify the source assets (at the root), instead of the ones that have been copied.
        // This is important because only the ones at the root are tracked in Git.
        std::string root{COOL_DEBUG_ONLY__CMAKE_SOURCE_DIR};
#else
        std::string root{exe_path::dir().string()};
#endif
        Cool::Path::initialize_root(root);
        Cool::Path::initialize_cool_res(root + "/" + COOL_PATH + "/res");
    };
};

} // namespace Cool
