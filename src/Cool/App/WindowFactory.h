#pragma once

#include "Window.h"

namespace Cool {

/// A wrapper for Glfw + Vulkan/OpenGL + ImGui initialization and shutdown and windows creation.
class WindowFactory {
public:
	/// Sets the OpenGL Version and initializes a bunch of stuff. /!\ You must use OpenGL 3.3 or greater for the current ImGui implementation.
	/// <param name="openGLMajorVersion">Open GL major version</param>
	/// <param name="openGLMinorVersion">OpenGL minor version</param>
	WindowFactory(int openGLMajorVersion, int openGLMinorVersion);
	~WindowFactory();

	// TODO update doc to talk about vulkan
	/// Creates a window and its OpenGL context. /!\ If you want to create multiple windows, make sure you create the main one last ! (the one you hand to AppManager)
	/// <param name="name">The name that will be displayed in the title bar of the window</param>
	/// <param name="width">Initial width of the window</param>
	/// <param name="height">Initial height of the window</param>
	/// <param name="windowToShareContextWith">Optional window. If set, the created window will share the same OpenGL context as this one.</param>
#ifdef __COOL_APP_VULKAN
	Window create(const char* name, int width, int height);
#endif
#ifdef __COOL_APP_OPENGL
	Window create(const char* name, int width, int height, GLFWwindow* windowToShareContextWith = NULL);
#endif

private:
	void initializeGLFW();
#ifdef __COOL_APP_OPENGL
	void setupGLDebugging();
#endif
	void setupImGui(Window& window);
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