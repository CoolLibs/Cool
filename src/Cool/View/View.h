#pragma once
#include <Cool/Events/MouveEventDispatcher.h>
#include <Cool/Gpu/RenderTarget.h>

namespace Cool {

class View {
public:
    View(std::string_view name)
        : _name{name}
    {
    }
    /**
     * @brief Displays the render target's image in an ImGui window
     * 
     * @param name The name of the window
     * @param image_texture_id 
     * @param image_size 
     * @param need_to_fit true iff the window and the image are expected to have different aspect ratios and a fitting operation is required 
     */
    void                     imgui_window(ImTextureID image_texture_id, ImageSize image_size, bool need_to_fit);
    void                     imgui_open_close_checkbox();
    std::optional<ImageSize> size() const { return _size; }

    void dispatch_mouse_move_event(const MouseMoveEvent<WindowCoordinates>& event, GLFWwindow* window);
    void dispatch_mouse_scroll_event(const MouseScrollEvent<WindowCoordinates>& event, GLFWwindow* window);
    void dispatch_mouse_button_event(const MouseButtonEvent<WindowCoordinates>& event, GLFWwindow* window);
    auto mouse_events() -> MouveEventDispatcher<ViewCoordinates>& { return _mouse_event_dispatcher; }

private:
    void grab_window_size();
    void grab_window_position();
    void display_image(ImTextureID image_texture_id, ImageSize image_size, bool need_to_fit);

    /**
     * @brief Returns the position relative to the ImGui window.
     * (0, 0) is in the bottom-left corner of the window
     */
    auto to_view_space(WindowCoordinates position, GLFWwindow* window) -> ViewCoordinates;

    /**
     * @brief Returns true iff the position is inside the view
     */
    bool contains(ViewCoordinates pos);

private:
    std::string                           _name              = "";
    bool                                  _is_open           = true;
    bool                                  _window_is_hovered = false;
    std::optional<ImageSize>              _size              = std::nullopt; // Can be nullopt when the window is closed
    ScreenCoordinates                     _position{};
    MouveEventDispatcher<ViewCoordinates> _mouse_event_dispatcher;
};

} // namespace Cool