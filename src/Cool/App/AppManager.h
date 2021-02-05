#pragma once

#include "OpenGLWindow.h"
#include "IApp.h"

struct ImGuiDockNode;

namespace Cool {
class Iapp;

/// Wrapper for an App. Handles the main window and the UI layout and docking.
class AppManager {
public:
	/// <param name="mainWindow">The main window where your app will be rendered to, created by the OpenGLWindowingSystem</param>
	/// <param name="app">An instance of an App class that you have to implement, deriving from IApp</param>
	AppManager(OpenGLWindow& mainWindow, IApp& app);
	~AppManager() = default;
	
	/// Run the app's update loop continuously
	void run();

	void onWindowMove(int x, int y);
	void onWindowResize(int w, int h);
	void update();

private:
	void updateAvailableRenderingSpaceSizeAndPos(ImGuiDockNode* node);
	bool onEvent(const SDL_Event& e); // Returns true iff the event has been handled and must not be forwarded to the app
	void ImGuiDockspace();

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

private:
	OpenGLWindow& m_mainWindow;
	IApp& m_app;
	bool m_bFirstFrame = true; // Used to prevent triggering the resize event twice at the start of the app
	bool m_bShowUI = true;
};

} // namespace Cool