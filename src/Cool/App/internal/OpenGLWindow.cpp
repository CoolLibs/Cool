#include "../OpenGLWindow.h"

namespace Cool {

OpenGLWindow::OpenGLWindow(GLFWwindow* window)
	: m_window(window) 
{}

OpenGLWindow::OpenGLWindow(OpenGLWindow&& o) noexcept 
	: m_window(o.m_window)
{
	o.m_window = nullptr;
}

OpenGLWindow::~OpenGLWindow() {
	if (m_window != nullptr) // Could have been moved
		glfwDestroyWindow(m_window);
}

bool OpenGLWindow::checkForFullscreenToggles(int key, int scancode, int action, int mods) {
	if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_F11) {
			switchFullScreen();
			return true;
		}
		if (key == GLFW_KEY_ESCAPE && m_bIsFullScreen) {
			escapeFullScreen();
			return true;
		}
	}
	return false;
}

void OpenGLWindow::switchFullScreen() {
	auto* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	if (m_bIsFullScreen)
		glfwSetWindowMonitor(m_window, NULL, m_posXBeforeFullscreen, m_posYBeforeFullscreen, m_widthBeforeFullscreen, m_heightBeforeFullscreen, mode->refreshRate);
	else {
		glfwGetWindowPos(m_window, &m_posXBeforeFullscreen, &m_posYBeforeFullscreen);
		glfwGetWindowSize(m_window, &m_widthBeforeFullscreen, &m_heightBeforeFullscreen);
		glfwSetWindowMonitor(m_window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
	}
	m_bIsFullScreen = !m_bIsFullScreen;
}

void OpenGLWindow::escapeFullScreen() {
	glfwSetWindowMonitor(m_window, NULL, 0, 0, 100, 100, 60);
	m_bIsFullScreen = false;
}

} // namespace Cool