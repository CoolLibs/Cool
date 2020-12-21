#pragma once

#include "Cool/App/internal/SDLOpenGLWrapper.h"
#include "Cool/App/internal/SDLGLWindow.h"

struct ImGuiDockNode;

namespace Cool {
class Iapp;

/// <summary>
/// Wrapper for an App. Handles the window and the UI layout.
/// </summary>
class AppManager {
public:
	/// <summary>
	/// 
	/// </summary>
	/// <param name="windowName">The name that will be displayed in the top bar of the window</param>
	/// <param name="windowDefaultWidth">The initial width of the window. Note that in Release builds, the window will open as maximized anyways.</param>
	/// <param name="windowDefaultHeight">The initial height of the window. Note that in Release builds, the window will open as maximized anyways.</param>
	AppManager(const char* windowName, int windowDefaultWidth = 1280, int windowDefaultHeight = 720);
	~AppManager();
	
	/// <summary>
	/// Run the app's update loop continuously
	/// </summary>
	/// <param name="app">An instance of an App class that you have to implement, deriving from Cool::IApp</param>
	/// <returns></returns>
	int run(Cool::IApp& app);

private:
	void update(Cool::IApp& app);
	inline bool shouldClose() const { return m_bShouldClose; }
	void onWindowMove();
	void onWindowResize();
	void updateAvailableRenderingSpaceSizeAndPos(ImGuiDockNode* node);
	bool onEvent(const SDL_Event& e); // Returns true iff the event has been handled and must not be forwarded to the app
	void ImGuiDockspace();
	inline void closeApp() { m_bShouldClose = true; }

private:
	SDLOpenGLWrapper m_SDLOpenGLWrapper; // We keep this as a member so that it will be destroyed on App shutdown and will shutdown SDL
	SDLGLWindow m_sdlglWindow;
	bool m_bShowUI = true;
	bool m_bShouldClose = false;
};

} // namespace Cool