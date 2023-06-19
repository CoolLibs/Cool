#pragma once
#include <Cool/Gpu/RenderTarget.h>
#include <Cool/Input/MouseEventDispatcher.h>
#include "Cool/Input/MouseCoordinates.h"
#include "Cool/Log/MessageId.h"

namespace Cool {

struct ViewWindowParams {
    bool                  fullscreen    = false;
    std::function<void()> extra_widgets = []() { // TODO(JF) If the widgets return true, don't dispatch events to the view
    };
};

struct ViewResizeEvent {};

/// A view is a window displaying an image.
/// You can subscribe to events
/// You can use one of the various subclasses to decide where your image will come from.
///
class View {
public:
    explicit View(std::string_view name, bool is_closable = false)
        : _name{name}
        , _is_closable{is_closable}
    {}

    virtual ~View()                      = default;
    View(View const&)                    = delete;
    auto operator=(View const&) -> View& = delete;
    View(View&&)                         = delete;
    auto operator=(View&&) -> View&      = delete;

    /// Displays the given texture in an ImGui window
    void imgui_window(ViewWindowParams const& = {});
    void imgui_open_close_toggle();

    /// Returns the size of the window.
    /// /!\ This is not the size of the image in the View, this is the size of the full window, including the potential margins around the image.
    auto window_size() const -> std::optional<img::Size> { return _window_size; }

    auto to_view_coordinates(ImGuiCoordinates) const -> ViewCoordinates;
    auto to_imgui_coordinates(ViewCoordinates) const -> ImGuiCoordinates;

    auto mouse_events() -> auto& { return _mouse_event_dispatcher; }
    // auto resize_event() -> auto& { return _resize_event_dispatcher; } // Probably not interesting. We might want to know when the actual image is resized, rather than the window.

    auto has_vertical_margins() const -> bool { return _window_size ? _has_vertical_margins : false; }

private: /// Child classes need to implement these functions in order for us to display their image in the View.
    virtual auto get_image_texture_id() const -> ImTextureID = 0;
    virtual auto get_image_size() const -> img::Size         = 0;

private:
    friend class AppManager;
    void dispatch_mouse_move_event(MouseMoveEvent<ImGuiCoordinates> const& event);
    void dispatch_mouse_scroll_event(MouseScrollEvent<ImGuiCoordinates> const& event);
    void dispatch_mouse_button_event(MouseButtonEvent<ImGuiCoordinates> const& event);

private:
    void store_window_size();
    void store_window_position();
    void display_image(ImTextureID image_texture_id, img::Size image_size);

    /// Returns true iff the position is inside the image fitted in the view.
    auto contains(ViewCoordinates) const -> bool;

private:
    std::string                           _name;
    bool                                  _is_closable;
    bool                                  _is_open           = true;
    bool                                  _window_is_hovered = false;
    std::optional<img::Size>              _window_size       = std::nullopt; // Can be nullopt when the window is closed
    ImGuiCoordinates                      _window_position{};
    MouseEventDispatcher<ViewCoordinates> _mouse_event_dispatcher;
    EventDispatcher<ViewResizeEvent>      _resize_event_dispatcher;
    RenderTarget                          _render_target;

    MessageId _log_position_message_id{};

    bool _has_vertical_margins{false};
};

} // namespace Cool