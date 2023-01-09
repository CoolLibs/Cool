#pragma once

#include "../WindowManager.h"
#include "WindowConfig.h"

namespace Cool {
/**
 * @brief A wrapper for (Glfw + Vulkan/OpenGL + ImGui)'s initialization and shutdown, as well as windows creation
 *
 */
template<typename RenderTarget_Impl>
class WindowFactory_Base {
public:
    WindowFactory_Base();
    ~WindowFactory_Base();

    WindowManager& window_manager() { return _window_manager; }

    /**
     * @brief Creates the main window. A main window is required for the application to run.
     * There can only be one such main window. If you want more windows, use make_secondary_window()
     * @return Window& A reference to the window (it is actually stored in the window_manager() who handles its lifetime).
     */
    Window& make_main_window(const WindowConfig& config);

    /**
     * @brief Creates a secondary window. It will not have an ImGui context.
     * You can create as many secondary windows as you want.
     * NB : you must also create one main window for the application to run.
     * @return Window& A reference to the window (it is actually stored in the window_manager() who handles its lifetime).
     */
    Window& make_secondary_window(const WindowConfig& config);

    /**
     * @brief Returns the underlying implementation for a given graphics API.
     * Note that if you use this your app wont be compatible with other graphics APIs (probably not a big deal, but better safe than sorry)
     *
     * @return RenderTarget_Impl&
     */
    RenderTarget_Impl& impl() { return _impl; }

#if HACK_RESET_IMGUI_CTX_EVERY_FRAME
    void reset()
    {
        _impl.reset(_window_manager.main_window());
    }
#endif

private:
    void        initialize_glfw();
    void        initialize_imgui();
    static void glfw_error_callback(int error, const char* description);

private:
    WindowManager     _window_manager;
    RenderTarget_Impl _impl;
};

} // namespace Cool

#include "WindowFactory_Base.tpp"