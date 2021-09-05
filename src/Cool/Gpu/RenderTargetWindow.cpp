#include "RenderTargetWindow.h"
#include <Cool/ImGuiExtras/ImGuiExtras.h>
#include <Cool/Image/ImageSizeU.h>

namespace Cool {

void RenderTargetWindow::imgui_window(std::string_view name, bool need_to_fit)
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
    // Display the image
    if (_size.has_value()) {
        const auto image_size = need_to_fit
                                    ? ImageSizeU::fit_into(*_size, _render_target.current_size())
                                    : static_cast<ImageSizeT<float>>(*_size);
        ImGuiExtras::image_centered(
            _render_target.imgui_texture_id(),
            {image_size.width(),
             image_size.height()});
        _is_hovered = ImGui::IsItemHovered();
    }
    else {
    }
    ImGui::End();
}

void RenderTargetWindow::update_render_target_size()
{
    _render_target.set_size(_preview_size.value_or(_size.value_or(ImageSize{})));
}

} // namespace Cool