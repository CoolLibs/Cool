#include "View.h"
#include <Cool/ImGui/ImGuiExtras.h>
#include <Cool/Image/ImageSizeU.h>

namespace Cool {

void View::imgui_window(ImTextureID image_texture_id, ImageSizeInsideView image_size_inside_view)
{
    if (_is_open)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0.f, 0.f}); // TODO add a parameter in the UI to control the padding specifically for the views
        ImGui::Begin(_name.c_str(), &_is_open, ImGuiWindowFlags_NoScrollbar);
        store_window_size();
        store_window_position();
        _window_is_hovered = ImGui::IsWindowHovered();
        display_image(image_texture_id, image_size_inside_view);
        ImGui::End();
        ImGui::PopStyleVar();
    }
    else
    {
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
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        return position.as_screen_coordinates(window);
    }
    else
    {
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

bool View::contains(ViewCoordinates pos, ImageSizeInsideView image_size)
{
    if (!_window_is_hovered)
    {
        return false;
    }
    else
    {
        if (_size)
        {
            const auto img_size   = image_size.fit_into(*_size);
            const auto pos_in_img = pos + glm::vec2{(img_size.width() - _size->width()) * 0.5f,
                                                    (img_size.height() - _size->height()) * 0.5f};
            return pos_in_img.x >= 0.f && pos_in_img.x <= img_size.width() &&
                   pos_in_img.y >= 0.f && pos_in_img.y <= img_size.height();
        }
        else
        {
            return false;
        }
    }
}

void View::dispatch_mouse_move_event(const ViewEvent<MouseMoveEvent<WindowCoordinates>>& event)
{
    const auto pos = to_view_space(event.event.position, event.window);
    _mouse_event_dispatcher.drag().dispatch_mouse_move_event({pos});
    if (contains(pos, event.image_size))
    {
        _mouse_event_dispatcher.move_event().dispatch({pos});
    }
}

void View::dispatch_mouse_scroll_event(const ViewEvent<MouseScrollEvent<WindowCoordinates>>& event)
{
    const auto pos = to_view_space(event.event.position, event.window);
    if (contains(pos, event.image_size))
    {
        _mouse_event_dispatcher.scroll_event().dispatch({.position = pos,
                                                         .dx       = event.event.dx,
                                                         .dy       = event.event.dy});
    }
}

void View::dispatch_mouse_button_event(const ViewEvent<MouseButtonEvent<WindowCoordinates>>& event)
{
    const auto pos          = to_view_space(event.event.position, event.window);
    const bool contains_pos = contains(pos, event.image_size);
    const auto new_event    = MouseButtonEvent<ViewCoordinates>{
           .position = pos,
           .button   = event.event.button,
           .action   = event.event.action,
           .mods     = event.event.mods};
    _mouse_event_dispatcher.drag().dispatch_mouse_button_event(new_event, contains_pos);
    if (contains_pos)
    {
        _mouse_event_dispatcher.button_event().dispatch(new_event);
    }
}

void View::store_window_size()
{
    const auto size = ImGui::GetContentRegionAvail();
    if (size.x >= 1.f && size.y >= 1.f)
    {
        const auto new_size = std::make_optional(img::Size{static_cast<img::Size::DataType>(size.x),
                                                           static_cast<img::Size::DataType>(size.y)});

        const bool has_changed = new_size != _size;
        _size                  = new_size;
        if (has_changed)
        {
            resize_event().dispatch({});
        }
    }
    else
    {
        _size.reset();
    }
}

void View::store_window_position()
{
    const auto pos = ImGui::GetCursorScreenPos();
    _position      = ScreenCoordinates{pos.x, pos.y};
}

void View::display_image(ImTextureID image_texture_id, ImageSizeInsideView _image_size_inside_view)
{
    if (_size.has_value())
    {
        const auto fitted_image_size = _image_size_inside_view.fit_into(*_size);
        ImGuiExtras::image_centered(image_texture_id, {fitted_image_size.width(), fitted_image_size.height()});
    }
}

} // namespace Cool