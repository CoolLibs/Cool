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

    void receive_mouse_move_event(const MouseMoveEvent<MainWindowCoordinates>& event, GLFWwindow* window);
    void receive_mouse_scroll_event(const MouseScrollEvent<MainWindowCoordinates>& event, GLFWwindow* window);
    void receive_mouse_button_event(const MouseButtonEvent<MainWindowCoordinates>& event, GLFWwindow* window);
    auto mouse_events() -> MouveEventDispatcher<ImGuiWindowCoordinates>& { return _mouse_event_dispatcher; }

private:
    void grab_window_size();
    void grab_window_position();
    void display_image(ImTextureID image_texture_id, ImageSize image_size, bool need_to_fit);

    /**
     * @brief Returns the position relative to the ImGui window, or std::nullopt if the position is outside of the view.
     * (0, 0) is in the bottom-left corner of the window
     * 
     * @param position 
     * @param window 
     */
    auto to_imgui_space(MainWindowCoordinates position, GLFWwindow* window) -> std::optional<ImGuiWindowCoordinates>;

private:
    std::string                                  _name    = "";
    bool                                         _is_open = true;
    std::optional<ImageSize>                     _size    = std::nullopt; // Can be nullopt when the window is closed
    ScreenCoordinates                            _position{};
    MouveEventDispatcher<ImGuiWindowCoordinates> _mouse_event_dispatcher;
};

} // namespace Cool