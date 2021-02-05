#pragma once

namespace Cool {

/// <summary>
/// A window and its OpenGL context
/// </summary>
class OpenGLWindow {
public:
	OpenGLWindow(GLFWwindow* m_window);
	OpenGLWindow(OpenGLWindow&&) noexcept;
	OpenGLWindow(const OpenGLWindow&) = delete;			   // Non-copyable because there should only be ONE owner of the window. Please store references to the window if you need to.
	OpenGLWindow& operator=(const OpenGLWindow&) = delete; // Non-copyable because there should only be ONE owner of the window. Please store references to the window if you need to.
	~OpenGLWindow();

	/// Returns the underlying glfw window pointer
	inline GLFWwindow* get() { return  m_window; }

	inline void makeCurrent() { glfwMakeContextCurrent(m_window); }
	inline void enableVSync() { glfwSwapInterval(1); }
	inline void disableVSync() { glfwSwapInterval(0); }
	bool checkForFullscreenToggles(const SDL_Event& e);
	void switchFullScreen();
	void escapeFullScreen();

private:
	GLFWwindow* m_window;
	bool m_bIsFullScreen = false;
};

} // namespace Cool