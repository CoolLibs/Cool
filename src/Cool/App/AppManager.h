#pragma once

#include <Cool/Core/should_we_use_a_separate_thread_for_update.h>
#include <Cool/Window/Window.h>
#include <Cool/Window/internal/WindowManager.h>
#include "AppManagerConfig.h"
#include "IApp.h"

namespace Cool {
class Iapp;

/// Wrapper for an App. Handles the main window and the UI layout and docking.
class AppManager {
public:
    /// <param name="mainWindow">The main window where your app will be rendered to, created by the WindowFactory</param>
    /// <param name="app">An instance of an App class that you have to implement, deriving from IApp</param>
    AppManager(Window& main_window, WindowManager& window_manager, IApp& app, AppManagerConfig config = {});
    ~AppManager();

    /// Runs the app's update loop continuously, until the user closes the main window
    void run();

private:
    void update();

    void ImGuiDockspace();

    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void key_callback_for_secondary_windows(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void window_close_callback_for_secondary_windows(GLFWwindow* window);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
    static void window_size_callback(GLFWwindow* window, int w, int h);
    static void window_pos_callback(GLFWwindow* window, int x, int y);

private:
    Window&          _main_window;
    WindowManager&   _window_manager;
    IApp&            m_app;
    bool             m_bShowUI = true;
    AppManagerConfig _config;
#if defined(COOL_UPDATE_APP_ON_SEPARATE_THREAD)
    std::thread _update_thread;
#endif
};

} // namespace Cool