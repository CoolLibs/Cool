#pragma once

/// <summary>
/// An SDL window and its OpenGL context
/// </summary>
struct SDLGLWindow {
	SDLGLWindow(SDL_Window* _window)
		: window(_window) 
	{
		glContext = SDL_GL_CreateContext(window);
		if (glContext == nullptr) {
			Log::Error("Failed to create OpenGL context : {}", SDL_GetError());
		}
	}
	/// <summary>
	/// Cleanup and free the memory
	/// </summary>
	inline void destroy() { SDL_DestroyWindow(window); }

	SDL_Window* window;
	SDL_GLContext glContext;

	inline void makeCurrent() { SDL_GL_MakeCurrent(window, glContext); }
	bool checkForFullscreenToggles(const SDL_Event& e);
	void switchFullScreen();
	void escapeFullScreen();

private:
	bool m_bIsFullScreen = false;
};