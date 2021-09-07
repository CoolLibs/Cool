#include "View.h"
#include <Cool/ImGuiExtras/ImGuiExtras.h>
#include <Cool/Image/ImageSizeU.h>

namespace Cool {

void View::imgui_window(ImTextureID image_texture_id, ImageSize image_size, bool need_to_fit)
{
    if (_is_open) {
        ImGui::Begin(_name.c_str(), &_is_open, ImGuiWindowFlags_NoScrollbar);
        // Update _size
        auto size = ImGui::GetContentRegionAvail();
        if (size.x >= 1.f && size.y >= 1.f) {
            _size.emplace(
                static_cast<ImageSize::DataType>(size.x),
                static_cast<ImageSize::DataType>(size.y));
        }
        else {
            _size.reset();
        }
        if (_size.has_value()) {
            // Display the image
            const auto fitted_image_size = need_to_fit
                                               ? ImageSizeU::fit_into(*_size, image_size)
                                               : static_cast<ImageSizeT<float>>(*_size);
            ImGuiExtras::image_centered(image_texture_id, {fitted_image_size.width(), fitted_image_size.height()});
            _is_hovered = ImGui::IsItemHovered();
        }
        else {
            _is_hovered = false;
        }
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

} // namespace Cool