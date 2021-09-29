#pragma once
#include <Cool/Input/KeyboardEvent.h>
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
    virtual void update(){};

    /**
     * @brief User inputs are only dispatched to the app when this returns true.
     * It can be usefull to set if to false for example while you are exporting a video and you want to prevent the user accidentally changing parameters.
     */
    virtual bool inputs_are_allowed() const { return true; }

    /**
     * @brief While this returns false the menu bar won't be shown
     */
    virtual bool wants_to_show_menu_bar() const { return true; }

    /**
     * @brief All ImGui windows should be created inside this function (it allows the dockspace to work properly)
     * 
     */
    virtual void ImGuiWindows(){};

    /**
     * @brief All ImGui menus created inside this function will appear in the main menu bar.
     * 
     */
    virtual void ImGuiMenus(){};

    /**
     * @brief This is where you receive and handle keyboard events.
     * 
     * @param event 
     */
    virtual void on_keyboard_event(const KeyboardEvent& event){};

    /**
     * @brief This is where you receive and handle mouse click events.
     * 
     * @param event 
     */
    virtual void on_mouse_button(const MouseButtonEvent<WindowCoordinates>& event){};

    /**
     * @brief This is where you receive and handle scroll events (which can be generated by a mouse or a touch pad with the pinch gesture).
     * 
     * @param event 
     */
    virtual void on_mouse_scroll(const MouseScrollEvent<WindowCoordinates>& event){};

    /**
     * @brief This is where you receive and handle mouse move events.
     * 
     * @param event 
     */
    virtual void on_mouse_move(const MouseMoveEvent<WindowCoordinates>& event){};
};

} // namespace Cool