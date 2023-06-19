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

    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void key_callback_for_secondary_windows(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void window_close_callback_for_secondary_windows(GLFWwindow* window);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

    void dispatch_all_events();
    void dispatch_scroll();

    template<typename Event>
    Cool::ViewEvent<Event> view_event(const Event& event, Cool::View const& view)
    {
        return {
            event,
            _window_manager.main_window().glfw(),
            {view.get_image_size_inside_view()}};
    }

private:
    WindowManager&   _window_manager;
    ViewsManager&    _views;
    IApp&            _app;
    AppManagerConfig _config;

    Cool::StyleEditor _style_editor{};
};

} // namespace Cool