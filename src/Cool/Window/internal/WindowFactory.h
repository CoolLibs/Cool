#pragma once
#include "../WindowManager.h"
#include "WindowConfig.h"
#include "WindowFactory_Impl.h"

namespace Cool {

/// A wrapper for [Glfw + Vulkan/OpenGL + ImGui]'s initialization and shutdown, as well as windows creation.
class WindowFactory {
public:
    WindowFactory();
    ~WindowFactory();
    WindowFactory(const WindowFactory&)                        = delete;
    auto operator=(const WindowFactory&) -> WindowFactory&     = delete;
    WindowFactory(WindowFactory&&) noexcept                    = delete;
    auto operator=(WindowFactory&&) noexcept -> WindowFactory& = delete;

    auto window_manager() -> WindowManager& { return _window_manager; }

    /// Creates the main window. A main window is required for the application to run.
    /// There can only be one such main window. If you want more windows, use `make_secondary_window()`.
    /// This returns a reference to the window (it is actually stored in the `window_manager()`, which handles its lifetime).
    auto make_main_window(WindowConfig const&) -> Window&;

    /// Creates a secondary window. It will not have an ImGui context.
    /// You can create as many secondary windows as you want.
    /// NB: you must also create one main window for the application to run.
    /// This returns a reference to the window (it is actually stored in the `window_manager()`, which handles its lifetime).
    auto make_secondary_window(WindowConfig const&) -> Window&;

private:
    WindowManager _window_manager;
};

} // namespace Cool