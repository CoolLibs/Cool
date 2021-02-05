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

//bool OpenGLWindow::checkForFullscreenToggles(const SDL_Event& e) {
//	if (e.type == SDL_KEYDOWN && e.window.windowID == SDL_GetWindowID(window)) {
//		if (e.key.keysym.scancode == SDL_SCANCODE_F11) {
//			switchFullScreen();
//			return true;
//		}
//		if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE && m_bIsFullScreen) {
//			escapeFullScreen();
//			return true;
//		}
//	}
//	return false;
//}
//
//void OpenGLWindow::switchFullScreen() {
//	if (m_bIsFullScreen)
//		SDL_SetWindowFullscreen(window, 0);
//	else
//		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
//	m_bIsFullScreen = !m_bIsFullScreen;
//}
//
//void OpenGLWindow::escapeFullScreen() {
//	SDL_SetWindowFullscreen(window, 0);
//	m_bIsFullScreen = false;
//}

} // namespace Cool