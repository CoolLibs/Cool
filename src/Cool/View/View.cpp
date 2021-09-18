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
        display_image(image_texture_id, image_size, need_to_fit);
        ImGui::End();
    }
    else {
        _size.reset();
    }
}

void View::imgui_open_close_checkbox()
{
    ImGui::Checkbox(_name.c_str(), &_is_open);
}

void View::receive_mouse_move_event(const MouseMoveEvent<MainWindowCoordinates>& event, GLFWwindow* window)
{
    const auto pos_screen = [&]() {
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            return event.position.as_screen_coordinates(window);
        }
        else {
            return ScreenCoordinates{event.position}; // We trick ImGui because if viewports are disabled, ImGui functions that pretend to return screen coordinates actually return window coordinates (this is a temporary measure because I know that ImGui plans on fixing this)
        }
    }();
    // Convert to this window's coordinate space
    auto pos = ImGuiWindowCoordinates{pos_screen - _position};
    // Check if we are hovering the image
    if (pos.x >= 0.f && pos.x <= _size.value_or(ImageSize{}).width() &&
        pos.y >= 0.f && pos.y <= _size.value_or(ImageSize{}).height()) {
        // Make y-axis point up
        pos.y = _size.value_or(ImageSize{}).height() - pos.y;
        //
        _mouse_event_dispatcher.move_event().receive({pos});
    }
}

void View::receive_mouse_scroll_event(const MouseScrollEvent<ScreenCoordinates>& event)
{
    // // Convert to this window's coordinate space
    // auto pos = ImGuiWindowCoordinates{event.position - _position};
    // // Check if we are hovering the image
    // if (pos.x >= 0.f && pos.x <= _size.value_or(ImageSize{}).width() &&
    //     pos.y >= 0.f && pos.y <= _size.value_or(ImageSize{}).height()) {
    //     // Make y-axis point up
    //     pos.y = _size.value_or(ImageSize{}).height() - pos.y;
    //     //
    //     _mouse_event_dispatcher.move_event().receive({pos});
    // }
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