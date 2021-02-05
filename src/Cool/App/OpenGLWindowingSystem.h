#pragma once

#include "OpenGLWindow.h"

namespace Cool {

/// A wrapper for Glfw, OpenGL and ImGui initialization and shutdown and windows creation.
class OpenGLWindowingSystem {
public:
	OpenGLWindowingSystem();
	~OpenGLWindowingSystem();

	/// <summary>
	/// Creates a window and its OpenGL context
	/// You have the responsiblity of freeing the memory (call openGlWindow.destroy())
	/// </summary>
	/// <param name="name">The name that will be displayed in the title bar of the window</param>
	/// <param name="defaultWidth">In Debug mode : this is the initial width of the window. In Release mode : the window will open as maximized and this value will only affect the size that users see when they un-maximize the window.</param>
	/// <param name="defaultHeight">In Debug mode : this is the initial height of the window. In Release mode : the window will open as maximized and this value will only affect the size that users see when they un-maximize the window.</param>
	OpenGLWindow createWindow(const char* name, int defaultWidth, int defaultHeight);

private:
	void initializeGLFW();
	void initializeSDLandOpenGL();
	void initializeImGui();
	void setupGLDebugging();
	void setupImGui(OpenGLWindow& openGLWindow);
	static void GlfwErrorCallback(int error, const char* description);

private:
#ifndef NDEBUG
	static bool s_bInitialized;
#endif
};

} // namespace Cool