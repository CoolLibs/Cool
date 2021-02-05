#pragma once

#include "SDLGLWindow.h"

namespace Cool {

class IApp;

/// <summary>
/// Handles the initialization of SDL and OpenGL and the creation of windows.
/// </summary>
class SDLOpenGLWrapper {
public:
	SDLOpenGLWrapper();
	~SDLOpenGLWrapper();

	/// <summary>
	/// Creates a window and its OpenGL context
	/// You have the responsiblity to free the memory (call openGlWindow.destroy())
	/// </summary>
	/// <param name="name">The name that will be displayed in the top bar of the window</param>
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