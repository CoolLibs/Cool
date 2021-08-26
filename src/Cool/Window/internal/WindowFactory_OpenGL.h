#pragma once

#if defined(__COOL_APP_OPENGL)

#include "WindowFactory_Base.h"
#include "Window_OpenGL.h"

namespace Cool {

class WindowFactory_OpenGL : public WindowFactory_Base {
public:
    /**
	 * @brief Sets the OpenGL Version and initializes a bunch of stuff. /!\ You must use OpenGL 3.3 or greater for the current ImGui implementation
	 * 
	 * @param openGLMajorVersion 
	 * @param openGLMinorVersion 
	 */
    WindowFactory_OpenGL(int openGLMajorVersion, int openGLMinorVersion);

    ~WindowFactory_OpenGL();

    /**
	 * @brief Creates a window and its OpenGL context.
	 * 
	 * @param name The name that will be displayed in the title bar of the window
	 * @param width Initial width of the window
	 * @param height Initial height of the window
	 * @return Window& 
	 */
    Window_OpenGL& make_main_window(const char* name, int width, int height, bool cap_framerate = true);
    Window_OpenGL& make_secondary_window(const char* name, int width, int height, bool cap_framerate = true);

private:
    void           setupGLDebugging();
    Window_OpenGL& make_window(const char* name, int width, int height, bool cap_framerate = true);
    void           setup_imgui(Window_OpenGL& window);

private:
    int m_openGLMajorVersion;
    int m_openGLMinorVersion;
    int m_openGLVersion;
};

} // namespace Cool

#endif