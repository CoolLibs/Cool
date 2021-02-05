#pragma once

namespace Cool {

	/// <summary>
	/// A window and its OpenGL context
	/// </summary>
	struct OpenGLWindow {
		OpenGLWindow(GLFWwindow* _window)
			: window(_window) {}
		/// <summary>
		/// Cleanup and free the memory
		/// </summary>
		inline void destroy() { glfwDestroyWindow(window); }

		GLFWwindow* window;

		inline void makeCurrent() { glfwMakeContextCurrent(window); }
		inline void enableVSync() { glfwSwapInterval(1); }
		inline void disableVSync() { glfwSwapInterval(0); }
		bool checkForFullscreenToggles(const SDL_Event& e);
		void switchFullScreen();
		void escapeFullScreen();

	private:
		bool m_bIsFullScreen = false;
	};

} // namespace Cool