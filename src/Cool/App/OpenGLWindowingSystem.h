#pragma once

#include "OpenGLWindow.h"

namespace Cool {

/// A wrapper for Glfw, OpenGL and ImGui initialization and shutdown and windows creation.
class OpenGLWindowingSystem {
public:
	/// Sets the OpenGL Version and initializes a bunch of stuff. /!\ You must use OpenGL 3.3 or greater for the current ImGui implementation.
	/// <param name="openGLMajorVersion">Open GL major version</param>
	/// <param name="openGLMinorVersion">OpenGL minor version</param>
	OpenGLWindowingSystem(int openGLMajorVersion, int openGLMinorVersion);
	~OpenGLWindowingSystem();

	/// Creates a window and its OpenGL context. /!\ If you want to create multiple windows, make sure you create the main one last ! (the one you hand to AppManager)
	/// <param name="name">The name that will be displayed in the title bar of the window</param>
	/// <param name="width">Initial width of the window</param>
	/// <param name="height">Initial height of the window</param>
	OpenGLWindow createWindow(const char* name, int width, int height);

private:
	void initializeGLFW();
	void setupGLDebugging();
	void setupImGui(OpenGLWindow& openGLWindow);
	static void GlfwErrorCallback(int error, const char* description);

private:
	int m_openGLMajorVersion;
	int m_openGLMinorVersion;
	int m_openGLVersion;
#ifndef NDEBUG
	static bool s_bInitialized;
#endif
};

} // namespace Cool