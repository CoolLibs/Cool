#pragma once

#include <Cool/View/ViewsManager.h>
#include <Cool/Window/Window.h>
#include <Cool/Window/WindowManager.h>
#include "AppManagerConfig.h"
#include "Cool/ImGui/StyleEditor.h"
#include "IApp.h"

namespace Cool {

/// Wrapper for an App. It handles the user events, the ImGui UI, and the windows' updates.
class AppManager {
public:
    /**
     * @param window_manager The WindowManager (the one that WindowFactory can give you)
     * @param app An instance of an App class that you have to implement, deriving from IApp.
     * @param config Configuration options that control the behaviour of the AppManager
     */
    AppManager(WindowManager& window_manager, ViewsManager& views, IApp& app, AppManagerConfig config);

    /// Runs the app's update loop continuously, until the user closes the main window.
    /// Also calls `on_update()` after every update.
    void run(std::function<void()> on_update);

private:
    void update();
    void restore_imgui_ini_state_ifn();
    void imgui_render(IApp&);
    void imgui_windows();
    void end_frame(WindowManager& window_manager);

    void request_rerender_thread_safe();

    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void key_callback_for_secondary_windows(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void window_close_callback_for_secondary_windows(GLFWwindow* window);

    void dispatch_all_events();
    void dispatch_mouse_movement();
    void dispatch_mouse_click();
    void dispatch_mouse_scroll();

private:
    WindowManager&   _window_manager;
    ViewsManager&    _views;
    IApp&            _app;
    AppManagerConfig _config;

    Cool::StyleEditor _style_editor{};

    int _frames_count{0};

    std::atomic<bool> _wants_to_request_rerender{false};
};

} // namespace Cool