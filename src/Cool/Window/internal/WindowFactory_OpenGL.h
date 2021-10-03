#pragma once
#if defined(COOL_OPENGL)

#include "WindowCreationParams.h"
#include "WindowManager.h"
#include "Window_OpenGL.h"

namespace Cool {

class WindowFactory_OpenGL {
public:
    WindowFactory_OpenGL();
    void shut_down(WindowManager& window_manager);

    void           setup_main_window(Window_OpenGL& window);
    void           setup_secondary_window(Window_OpenGL& window, WindowManager& window_manager);
    Window_OpenGL& make_window(const WindowCreationParams& params, WindowManager& window_manager);

private:
    void setupGLDebugging();
    void setup_imgui(Window_OpenGL& window);
};

} // namespace Cool

#endif