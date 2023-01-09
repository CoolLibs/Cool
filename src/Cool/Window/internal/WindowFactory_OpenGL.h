#pragma once
#if defined(COOL_OPENGL)

#include "../WindowManager.h"
#include "WindowConfig.h"
#include "Window_OpenGL.h"

namespace Cool {

class WindowFactory_OpenGL {
public:
    WindowFactory_OpenGL();
    static void shut_down(WindowManager& window_manager);

    void           setup_main_window(Window_OpenGL& window);
    static void    setup_secondary_window(Window_OpenGL&, WindowManager& window_manager);
    Window_OpenGL& make_window(const WindowConfig& config, WindowManager& window_manager);

#if HACK_RESET_IMGUI_CTX_EVERY_FRAME
    void reset(Window_OpenGL& main_window);
#endif

private:
    void setupGLDebugging();
    void setup_imgui(Window_OpenGL& window);
    static void shutdown_imgui();
};

} // namespace Cool

#endif