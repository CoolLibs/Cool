#pragma once
#include <Cool/Input/MouseButtonEvent.h>
#include <Cool/Input/MouseMoveEvent.h>
#include <Cool/Input/MouseScrollEvent.h>

namespace Cool {

class IApp {
public:
    /**
     * @brief Update function that will be called repeatedly
     *
     */
    virtual void update() = 0;

    /// Called when a global setting changed and the app will need to rerender.
    /// (Useful if the app has optimizations that only make it render when something changes).
    virtual void request_rerender(){};

    /**
     * @brief User inputs are only dispatched to the app when this returns true.
     * It can be useful to set if to false for example while you are exporting a video and you want to prevent the user accidentally changing parameters.
     */
    virtual bool inputs_are_allowed() const = 0;

    /**
     * @brief While this returns false the menu bar won't be shown
     */
    virtual bool wants_to_show_menu_bar() const = 0;

    /**
     * @brief All ImGui windows should be created inside this function (it allows the dockspace to work properly)
     *
     */
    virtual void imgui_windows() = 0;

    /**
     * @brief All ImGui menus created inside this function will appear in the main menu bar.
     *
     */
    virtual void imgui_menus() = 0;

    /// This is called before everything is saved by the serialization (unlike the IApp's destructor, which is called afterward, and might also be called at startup if loading the serialization fails.).
    virtual void on_shutdown() = 0;

    // /!\ This has no effect if a state has already been saved and not been restored yet.
    void save_imgui_windows_state();
    void restore_imgui_windows_state();

private:
    friend class AppManager;
    std::optional<std::string> _imgui_ini_state_to_restore{};
    bool                       _wants_to_restore_ini_state{false};
};

} // namespace Cool