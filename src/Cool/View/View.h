#pragma once
#include <Cool/Input/MouveEventDispatcher.h>
#include "ViewEvent.h"

namespace Cool {

class View {
public:
    explicit View(std::string_view name)
        : _name{name}
    {
    }
    /**
     * @brief Displays the render target's image in an ImGui window
     * 
     * @param name The name of the window
     */
    void                     imgui_window(ImTextureID image_texture_id, ImageSizeInsideView _image_size_inside_view);
    void                     imgui_open_close_checkbox();
    std::optional<img::Size> size() const { return _size; }

    void dispatch_mouse_move_event(const ViewEvent<MouseMoveEvent<WindowCoordinates>>& event);
    void dispatch_mouse_scroll_event(const ViewEvent<MouseScrollEvent<WindowCoordinates>>& event);
    void dispatch_mouse_button_event(const ViewEvent<MouseButtonEvent<WindowCoordinates>>& event);
    auto mouse_events() -> auto& { return _mouse_event_dispatcher; }
    auto resize_event() -> auto& { return _resize_event_dispatcher; }

private:
    void store_window_size();
    void store_window_position();
    void display_image(ImTextureID image_texture_id, ImageSizeInsideView _image_size_inside_view);

    /**
     * @brief Returns the position relative to the ImGui window.
     * (0, 0) is in the bottom-left corner of the window
     */
    auto to_view_space(WindowCoordinates position, GLFWwindow* window) -> ViewCoordinates;

    /**
     * @brief Returns true iff the position is inside the image fitted in the view
     */
    bool contains(ViewCoordinates pos, ImageSizeInsideView image_size);

private:
    std::string                           _name              = "";
    bool                                  _is_open           = true;
    bool                                  _window_is_hovered = false;
    std::optional<img::Size>              _size              = std::nullopt; // Can be nullopt when the window is closed
    ScreenCoordinates                     _position{};
    MouveEventDispatcher<ViewCoordinates> _mouse_event_dispatcher;
    EventDispatcher<ViewResizeEvent>      _resize_event_dispatcher;
};

} // namespace Cool