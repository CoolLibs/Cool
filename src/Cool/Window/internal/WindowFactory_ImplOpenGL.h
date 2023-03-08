#pragma once
#if defined(COOL_OPENGL)

#include "../Window.h"
#include "../WindowManager.h"
#include "WindowConfig.h"

namespace Cool {

struct WindowFactory_ImplOpenGL {
    static void shut_down(WindowManager& window_manager);
    static void setup_main_window(Window& window);
    static void setup_secondary_window(Window&, WindowManager& window_manager);
    static auto make_window(WindowConfig const& config, WindowManager& window_manager) -> Window&;
};

} // namespace Cool

#endif