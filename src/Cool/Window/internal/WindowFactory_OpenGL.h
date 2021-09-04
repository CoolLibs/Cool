#pragma once
#if defined(__COOL_APP_OPENGL)

#include "WindowCreationParams.h"
#include "WindowManager.h"
#include "Window_OpenGL.h"

namespace Cool {

class WindowFactory_OpenGL {
public:
    /**
	 * @brief Sets the OpenGL Version and initializes a bunch of stuff. /!\ You must use OpenGL 3.3 or greater for the current ImGui implementation
	 * 
	 * @param openGLMajorVersion 
	 * @param openGLMinorVersion 
	 */
    WindowFactory_OpenGL(int openGLMajorVersion = 3, int openGLMinorVersion = 3);
    void shut_down(WindowManager& window_manager);

    void           setup_main_window(Window_OpenGL& window);
    void           setup_secondary_window(Window_OpenGL& window, WindowManager& window_manager);
    Window_OpenGL& make_window(const WindowCreationParams& params, WindowManager& window_manager);

private:
    void setupGLDebugging();
    void setup_imgui(Window_OpenGL& window);

private:
    int m_openGLMajorVersion;
    int m_openGLMinorVersion;
    int m_openGLVersion;
};

} // namespace Cool

#endif