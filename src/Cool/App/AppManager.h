#pragma once

#include "OpenGLWindow.h"
#include "IApp.h"

struct ImGuiDockNode;

namespace Cool {
class Iapp;

/// <summary>
/// Wrapper for an App. Handles the main window and the UI layout.
/// </summary>
class AppManager {
public:
	AppManager(OpenGLWindow& mainWindow);
	~AppManager() = default;
	
	/// <summary>
	/// Run the app's update loop continuously
	/// </summary>
	/// <param name="app">An instance of an App class that you have to implement, deriving from Cool::IApp</param>
	/// <returns></returns>
	int run(Cool::IApp& app);

	void onWindowMove(int x, int y);
	void onWindowResize(int w, int h);
	void update();

private:
	inline bool shouldClose() const { return m_bShouldClose; }
	void updateAvailableRenderingSpaceSizeAndPos(ImGuiDockNode* node);
	bool onEvent(const SDL_Event& e); // Returns true iff the event has been handled and must not be forwarded to the app
	void ImGuiDockspace();
	inline void closeApp() { m_bShouldClose = true; }

private:
	OpenGLWindow& m_mainWindow;
	bool m_bFirstFrame = true; // Used to prevent triggering the resize event twice at the start of the app
	bool m_bShowUI = true;
	bool m_bShouldClose = false;
	Cool::IApp* app;
};

} // namespace Cool