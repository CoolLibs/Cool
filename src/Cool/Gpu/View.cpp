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
        _is_hovered = false;
    }
}

void View::imgui_open_close_checkbox()
{
    ImGui::Checkbox(_name.c_str(), &_is_open);
}

void View::receive_mouse_move_event(const MouseMoveEvent<ScreenCoordinates>& event)
{
    if (_is_hovered) {
        // Convert to this window's position
        auto pos = ImGuiWindowCoordinates{
            event.position - glm::vec2{_position.x, _position.y}};
        // Make y-axis point up
        pos.y = _size.value_or(ImageSize{}).height() - pos.y;
        //
        _mouse_event_dispatcher.move_event().receive({pos});
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
    _position = ImGui::GetCursorScreenPos();
}

void View::display_image(ImTextureID image_texture_id, ImageSize image_size, bool need_to_fit)
{
    if (_size.has_value()) {
        const auto fitted_image_size = need_to_fit
                                           ? ImageSizeU::fit_into(*_size, image_size)
                                           : static_cast<ImageSizeT<float>>(*_size);
        ImGuiExtras::image_centered(image_texture_id, {fitted_image_size.width(), fitted_image_size.height()});
        _is_hovered = ImGui::IsItemHovered();
    }
    else {
        _is_hovered = false;
    }
}

} // namespace Cool