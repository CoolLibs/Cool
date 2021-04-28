#pragma once

#ifdef __COOL_APP_VULKAN
#include "internal/VulkanWindowState.h"
#include "internal/VulkanContext.h"
#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#endif
#include <GLFW/glfw3.h>

namespace Cool {

/// <summary>
/// A window and its OpenGL or Vulkan context
/// </summary>
class Window {
public:
#ifdef __COOL_APP_VULKAN
	Window(Window&&) noexcept;
#endif
#ifdef __COOL_APP_OPENGL
	Window(Window&&) noexcept;
#endif
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

#ifdef __COOL_APP_VULKAN
	void check_for_swapchain_rebuild();
	void FramePresent();
	void FrameRender(ImDrawData* draw_data);
#endif

private:
	void setSwapInterval(int value);
	friend class WindowFactory;
	/// <summary>
	/// Please use WindowFactory::create() to create a Window
	/// </summary>
#ifdef __COOL_APP_VULKAN
	Window(GLFWwindow* m_window, VulkanContext& vulkan_context);
#endif
#ifdef __COOL_APP_OPENGL
	Window(GLFWwindow* m_window);
#endif

private:
	GLFWwindow* m_window;
	bool m_bIsFullScreen = false;
	int m_posXBeforeFullscreen;
	int m_posYBeforeFullscreen;
	int m_widthBeforeFullscreen;
	int m_heightBeforeFullscreen;
	bool m_bIsVSyncEnabled = true;

#ifdef __COOL_APP_VULKAN
public:
	VulkanWindowState _vulkan_window_state;
	VulkanContext& _vulkan_context;
#endif
};

} // namespace Cool