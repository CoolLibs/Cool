#include "View.h"
#include <Cool/ImGuiExtras/ImGuiExtras.h>
#include <Cool/Image/ImageSizeU.h>

namespace Cool {

void View::imgui_window(ImTextureID image_texture_id, ImageSize image_size, bool need_to_fit)
{
    if (_is_open) {
        ImGui::Begin(_name.c_str(), &_is_open, ImGuiWindowFlags_NoScrollbar);
        grab_window_size();
        grab_window_position();
        _window_is_hovered = ImGui::IsWindowHovered();
        display_image(image_texture_id, image_size, need_to_fit);
        ImGui::End();
    }
    else {
        _size.reset();
        _window_is_hovered = false;
    }
}

void View::imgui_open_close_checkbox()
{
    ImGui::Checkbox(_name.c_str(), &_is_open);
}

static ScreenCoordinates window_to_screen(WindowCoordinates position, GLFWwindow* window)
{
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        return position.as_screen_coordinates(window);
    }
    else {
        return ScreenCoordinates{position}; // We trick ImGui because if viewports are disabled, ImGui functions that pretend to return screen coordinates actually return window coordinates (this is a temporary measure because I know that ImGui plans on fixing this)
    }
}

static ViewCoordinates screen_to_view(ScreenCoordinates position, ScreenCoordinates window_position, float height)
{
    const auto pos = ViewCoordinates{position - window_position};
    return ViewCoordinates{
        pos.x,
        height - pos.y}; // Make y-axis point up
}

auto View::to_view_space(WindowCoordinates position, GLFWwindow* window) -> ViewCoordinates
{
    return screen_to_view(
        window_to_screen(position, window),
        _position,
        _size ? static_cast<float>(_size->height()) : 0.f);
}

bool View::contains(ViewCoordinates pos)
{
    if (!_window_is_hovered) {
        return false;
    }
    else {
        if (_size) {
            const auto size = static_cast<ImageSizeT<float>>(*_size);
            return pos.x >= 0.f && pos.x <= size.width() &&
                   pos.y >= 0.f && pos.y <= size.height();
        }
        else {
            return false;
        }
    }
}

void View::dispatch_mouse_move_event(const MouseMoveEvent<WindowCoordinates>& event, GLFWwindow* window)
{
    const auto pos = to_view_space(event.position, window);
    _mouse_event_dispatcher.drag().dispatch_mouse_move_event({pos});
    if (contains(pos)) {
        _mouse_event_dispatcher.move_event().dispatch({pos});
    }
}

void View::dispatch_mouse_scroll_event(const MouseScrollEvent<WindowCoordinates>& event, GLFWwindow* window)
{
    const auto pos = to_view_space(event.position, window);
    if (contains(pos)) {
        _mouse_event_dispatcher.scroll_event().dispatch({event.dx, event.dy, pos});
    }
}

void View::dispatch_mouse_button_event(const MouseButtonEvent<WindowCoordinates>& event, GLFWwindow* window)
{
    const auto pos          = to_view_space(event.position, window);
    const bool contains_pos = contains(pos);
    const auto new_event    = MouseButtonEvent<ViewCoordinates>{pos, event.button, event.action, event.mods};
    _mouse_event_dispatcher.drag().dispatch_mouse_button_event(new_event, contains_pos);
    if (contains_pos) {
        _mouse_event_dispatcher.button_event().dispatch(new_event);
    }
}

void View::grab_window_size()
{
    auto size = ImGui::GetContentRegionAvail();
    if (size.x >= 1.f && size.y >= 1.f) {
        _size.emplace(
            static_cast<ImageSize::DataType>(size.x),
            static_cast<ImageSize::DataType>(size.y));
    }
    else {
        _size.reset();
    }
}

void View::grab_window_position()
{
    const auto pos = ImGui::GetCursorScreenPos();
    _position      = ScreenCoordinates{pos.x, pos.y};
}

void View::display_image(ImTextureID image_texture_id, ImageSize image_size, bool need_to_fit)
{
    if (_size.has_value()) {
        const auto fitted_image_size = need_to_fit
                                           ? ImageSizeU::fit_into(*_size, image_size)
                                           : static_cast<ImageSizeT<float>>(*_size);
        ImGuiExtras::image_centered(image_texture_id, {fitted_image_size.width(), fitted_image_size.height()});
    }
}

} // namespace Cool