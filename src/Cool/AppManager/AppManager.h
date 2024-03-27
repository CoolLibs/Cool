#pragma once
#include <Cool/View/ViewsManager.h>
#include <GLFW/glfw3.h>
#include "AppManagerConfig.h"
#include "Cool/ImGui/StyleEditor.h"
#include "IApp.h"

namespace Cool {

/// Wrapper for an App. It handles the user events, the ImGui UI, and the windows' updates.
class AppManager {
public:
    /**
     * @param app An instance of an App class that you have to implement, deriving from IApp.
     * @param config Configuration options that control the behaviour of the AppManager
     */
    AppManager(ViewsManager& views, IApp& app, AppManagerConfig config);

    /// Runs the app's update loop continuously, until the user closes the window.
    /// Also calls `on_update()` after every update.
    void run(std::function<void()> const& on_update);

private:
    void update();
    void restore_imgui_ini_state_ifn();
    void imgui_render(IApp&);
    void imgui_windows();
    void end_frame();

    static void key_callback(GLFWwindow* glfw_window, int key, int scancode, int action, int mods);

    void dispatch_all_events();
    void dispatch_mouse_movement();
    void dispatch_mouse_click();
    void dispatch_mouse_scroll();

private:
    ViewsManager&    _views;
    IApp&            _app;
    AppManagerConfig _config;

    Cool::StyleEditor _style_editor{};

    int _frames_count{0};
};

} // namespace Cool