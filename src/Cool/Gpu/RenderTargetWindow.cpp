#include "RenderTargetWindow.h"
#include <Cool/ImGuiExtras/ImGuiExtras.h>
#include <Cool/Image/ImageSizeU.h>

namespace Cool {

void RenderTargetWindow::imgui_window(std::string_view name)
{
    ImGui::Begin(name.data(), nullptr, ImGuiWindowFlags_NoScrollbar);
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
    // if (!_render_target.constrained_size()) {
    //     _render_target.set_constrained_size(_size);
    //     _is_aspect_ratio_constrained = false;
    // }
    // Display the image
    if (_size.has_value()) {
        const auto image_size = _is_aspect_ratio_constrained
                                    ? ImageSizeU::fit_into(*_size, _render_target.current_size())
                                    : static_cast<ImageSizeT<float>>(*_size);
        ImGuiExtras::image_centered(
            _render_target.imgui_texture_id(),
            {image_size.width(),
             image_size.height()});
        // Update _is_hovered
        _is_hovered = ImGui::IsItemHovered();
    }
    else {
    }
    ImGui::End();
}

} // namespace Cool