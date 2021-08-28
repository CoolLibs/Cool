#include "RenderTarget_Base.h"

#if defined(__COOL_APP_VULKAN)
#include "../Vulkan/RenderTarget_ImplVulkan.h"
template class Cool::RenderTarget_Base<Cool::Vulkan::RenderTarget>;
#elif defined(__COOL_APP_OPENGL)
#include "RenderTarget_ImplOpenGL.h"
template class Cool::RenderTarget_Base<Cool::RenderTarget_ImplOpenGL>;
#endif

namespace Cool {

template<typename T>
void RenderTarget_Base<T>::resize_if_necessary()
{
    if (_imgui_window_size.x != width() || _imgui_window_size.y != height()) {
        _impl.resize(_imgui_window_size.x, _imgui_window_size.y);
    }
}

template<typename T>
void RenderTarget_Base<T>::imgui_window() const
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