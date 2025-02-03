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

namespace Cool {

struct RunConfig {
    std::vector<WindowConfig> windows_configs{};
    AppManagerConfig          app_manager_config{};
};

namespace internal {
void run_impl(
    int argc, char** argv, RunConfig const& config,
    std::function<void()> const&                                                                         initialize_paths_config,
    std::function<std::unique_ptr<IApp>(Cool::WindowManager& windows, Cool::ViewsManager& views)> const& make_unique_app
);
}

template<typename App, typename PathsConfigT = PathsConfig>
void run(int argc, char** argv, RunConfig const& config)
{
    internal::run_impl(
        argc, argv, config,
        []() { Path::initialize<PathsConfigT>(); },
        [](Cool::WindowManager& windows, Cool::ViewsManager& views) { return std::make_unique<App>(windows, views); }
    );
}

} // namespace Cool
