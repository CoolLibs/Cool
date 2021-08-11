#pragma once

#include <Cool/Window/Window.h>
#include <Cool/Window/WindowFactory.h>
#include "IApp.h"

struct ImGuiDockNode;

namespace Cool {
class Iapp;

/// Wrapper for an App. Handles the main window and the UI layout and docking.
class AppManager {
public:
    /// <param name="mainWindow">The main window where your app will be rendered to, created by the WindowFactory</param>
    /// <param name="app">An instance of an App class that you have to implement, deriving from IApp</param>
    AppManager(Window& mainWindow, std::list<Window>& windows, IApp& app);
    ~AppManager();

    /// Runs the app's update loop continuously, until the user closes the main window
    void run();

    /// <summary>
    /// Will prevent ImGui from reacting to key events (NB : doesn't affect text inputs, only things like moving in a list with up / down arrows).
    /// This is off by default;
    /// </summary>
    inline void DontForwardKeyEventsToImGui() { m_bDoForwardKeyEventsToImGui = false; }
    /// <summary>
    /// Will allow ImGui to react to key events (NB : doesn't affect text inputs, only things like moving in a list with up / down arrows).
    /// This is on by default;
    /// </summary>
    inline void DoForwardKeyEventsToImGui() { m_bDoForwardKeyEventsToImGui = true; }

private:
    void update();
    void onWindowMove(int x, int y);
    void onWindowResize(int w, int h);

    void ImGuiDockspace();
    void updateAvailableRenderingSpaceSizeAndPos(ImGuiDockNode* node);

    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
    static void window_size_callback(GLFWwindow* window, int w, int h);
    static void window_pos_callback(GLFWwindow* window, int x, int y);

private:
    Window&            _main_window;
    std::list<Window>& _windows;
    IApp&              m_app;
    bool               m_bFirstFrame                = true; // Used to prevent triggering the resize event twice at the start of the app
    bool               m_bShowUI                    = true;
    bool               m_bDoForwardKeyEventsToImGui = true;
    std::thread        _update_thread;
};

} // namespace Cool