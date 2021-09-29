#pragma once

#include <Cool/Window/Window.h>
#include <Cool/Window/internal/WindowManager.h>
#include "AppManagerConfig.h"
#include "IApp.h"
#include "should_we_use_a_separate_thread_for_update.h"

namespace Cool {

/**
 * @brief Wrapper for an App. It handles the user events, the ImGui UI, and the windows' updates.
 * 
 */
class AppManager {
public:
    /**
     * @param window_manager The WindowManager (the one that WindowFactory can give you)
     * @param app An instance of an App class that you have to implement, deriving from IApp.
     * @param config Configuration options that control the behaviour of the AppManager
     */
    AppManager(WindowManager& window_manager, IApp& app, AppManagerConfig config = {});
    ~AppManager();

    /**
     * @brief Runs the app's update loop continuously, until the user closes the main window
     * 
     */
    void run();

private:
    void update();

    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void key_callback_for_secondary_windows(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void window_close_callback_for_secondary_windows(GLFWwindow* window);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
    static void window_size_callback(GLFWwindow* window, int w, int h);
    static void window_pos_callback(GLFWwindow* window, int x, int y);

private:
    WindowManager&   _window_manager;
    IApp&            _app;
    AppManagerConfig _config;
#if defined(COOL_UPDATE_APP_ON_SEPARATE_THREAD)
    std::thread _update_thread;
#endif
};

} // namespace Cool