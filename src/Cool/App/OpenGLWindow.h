#pragma once

namespace Cool {

/// <summary>
/// A window and its OpenGL context
/// </summary>
class OpenGLWindow {
public:
	OpenGLWindow(OpenGLWindow&&) noexcept;
	OpenGLWindow(const OpenGLWindow&) = delete;			   // Non-copyable because there should only be ONE owner of the window. Please store references to the window if you need to.
	OpenGLWindow& operator=(const OpenGLWindow&) = delete; // Non-copyable because there should only be ONE owner of the window. Please store references to the window if you need to.
	~OpenGLWindow();

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

private:
	friend class OpenGLWindowingSystem;
	/// <summary>
	/// Please use OpenGLWindowingSystem::createWindow() to create an OpenGLWindow
	/// </summary>
	OpenGLWindow(GLFWwindow* m_window); 

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