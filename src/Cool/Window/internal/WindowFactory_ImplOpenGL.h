#pragma once
#if defined(COOL_OPENGL)

#include "../WindowManager.h"
#include "WindowConfig.h"
#include "Window_OpenGL.h"

namespace Cool {

struct WindowFactory_ImplOpenGL {
    static void shut_down(WindowManager& window_manager);
    static void setup_main_window(Window_OpenGL& window);
    static void setup_secondary_window(Window_OpenGL&, WindowManager& window_manager);
    static auto make_window(WindowConfig const& config, WindowManager& window_manager) -> Window_OpenGL&;
};

} // namespace Cool

#endif