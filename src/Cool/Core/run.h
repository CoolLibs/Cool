#pragma once
#include <Cool/Serialization/Serialization.h>
#include "Cool/AppManager/AppManagerConfig.h"
#include "Cool/AppManager/IApp.h"
#include "Cool/Log/OptionalErrorMessage.h"
#include "Cool/Path/Path.h"
#include "Cool/Path/PathsConfig.h"
#include "Cool/View/ViewsManager.h"
#include "Cool/Window/WindowManager.h"
#include "Cool/Window/internal/WindowConfig.h"
#include "hide_console_in_release.h"
//
#include <ser20/archives/json.hpp> // Must be included last because the more files it sees, the more it slows down compilation (by A LOT)

namespace Cool {

struct RunConfig {
    std::vector<WindowConfig> windows_configs{};
    AppManagerConfig          app_manager_config{};
};

namespace internal {
void run_impl(
    int argc, char** argv, RunConfig const& config,
    std::function<void()> const&                                                                         initialize_paths_config,
    std::function<std::unique_ptr<IApp>(Cool::WindowManager& windows, Cool::ViewsManager& views)> const& make_unique_app,
    std::function<OptionalErrorMessage(IApp&)> const&                                                    load_app,
    std::function<void(IApp const&)> const&                                                              save_app
);
}

template<typename App, typename PathsConfigT = PathsConfig>
void run(int argc, char** argv, RunConfig const& config)
{
    auto const app_serialization_path = []() { // Needs to be a lambda because we cannot call it yet because Path has not been initialized
        return Cool::Path::user_data() / "last-session.json";
    };
    internal::run_impl(
        argc, argv, config,
        []() { Path::initialize<PathsConfigT>(); },
        [](Cool::WindowManager& windows, Cool::ViewsManager& views) { return std::make_unique<App>(windows, views); },
        [&](IApp& app) { return Serialization::load<App, ser20::JSONInputArchive>(static_cast<App&>(app), app_serialization_path()); },
        [&](IApp const& app) { Serialization::save<App, ser20::JSONOutputArchive>(static_cast<App const&>(app), app_serialization_path(), "App"); }
    );
}

} // namespace Cool
