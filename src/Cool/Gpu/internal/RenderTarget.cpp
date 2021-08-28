#include "RenderTarget.h"

namespace Cool {

void RenderTarget::resize_if_necessary()
{
    if (_imgui_window_size.x != width() || _imgui_window_size.y != height()) {
        _impl.resize(_imgui_window_size.x, _imgui_window_size.y);
    }
}

void RenderTarget::imgui_window() const
{
    ImGui::Begin("MyImage", nullptr, ImGuiWindowFlags_NoScrollbar);
    auto size = ImGui::GetContentRegionAvail();
    if (size.x > 0.f && size.y > 0.f) {
        _imgui_window_size = {static_cast<uint32_t>(size.x), static_cast<uint32_t>(size.y)};
    }
    ImGui::Image(
        _impl.imgui_texture_id(),
        {static_cast<float>(width()),
         static_cast<float>(height())});
    ImGui::End();
}

} // namespace Cool