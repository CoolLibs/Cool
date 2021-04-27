#pragma once

#ifdef __COOL_APP_VULKAN
#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#endif
#include <GLFW/glfw3.h>

namespace Cool {

/// <summary>
/// A window and its OpenGL context
/// </summary>
class Window {
public:
	Window(Window&&) noexcept;
	Window(const Window&) = delete;			   // Non-copyable because there should only be ONE owner of the window. Please store references to the window if you need to.
	Window& operator=(const Window&) = delete; // Non-copyable because there should only be ONE owner of the window. Please store references to the window if you need to.
	~Window();

	/// <summary>
	/// Returns the underlying glfw window pointer
	/// </summary>
	inline GLFWwindow* get() { return  m_window; }
	/// <summary>
	/// Returns the monitor that the window is mostly displayed in
	/// </summary>
	GLFWmonitor* getCurrentMonitor() const;

	inline void makeCurrent() { glfwMakeContextCurrent(m_window); }
	void enableVSync();
	void disableVSync();
	inline bool isVSyncEnabled() const { return m_bIsVSyncEnabled; }
	/// <summary>
	/// You have to forward the parameters of the IApp::onKeyboardEvent() function (a.k.a. glfw key callback) and it will turn fullscreen on / off appropriately.
	/// </summary>
	/// <returns>true iff something has changed</returns>
	bool checkForFullscreenToggles(int key, int scancode, int action, int mods);
	void switchFullScreen();
	void escapeFullScreen();

	void check_for_swapchain_rebuild();

private:
	void setSwapInterval(int value);
	friend class WindowFactory;
	/// <summary>
	/// Please use WindowFactory::create() to create a Window
	/// </summary>
	Window(GLFWwindow* m_window);

private:
	GLFWwindow* m_window;
	bool m_bIsFullScreen = false;
	int m_posXBeforeFullscreen;
	int m_posYBeforeFullscreen;
	int m_widthBeforeFullscreen;
	int m_heightBeforeFullscreen;
	bool m_bIsVSyncEnabled = true;
};

} // namespace Cool