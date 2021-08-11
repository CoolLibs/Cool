#pragma once

#if defined(__COOL_APP_OPENGL)

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
    WindowFactory_OpenGL(int openGLMajorVersion, int openGLMinorVersion);

    /**
	 * @brief Creates a window and its OpenGL context. /!\ If you want to create multiple windows, make sure you create the main one last ! (the one you hand to AppManager)
	 * 
	 * @param name The name that will be displayed in the title bar of the window
	 * @param width Initial width of the window
	 * @param height Initial height of the window
	 * @return Window& 
	 */
    Window_OpenGL&            make_window(const char* name, int width, int height);
    std::list<Window_OpenGL>& windows() { return _windows; }

private:
    void setupGLDebugging();
    void setup_imgui(GLFWwindow* glfw_window);

private:
    std::list<Window_OpenGL> _windows;

    int m_openGLMajorVersion;
    int m_openGLMinorVersion;
    int m_openGLVersion;
};

} // namespace Cool

#endif