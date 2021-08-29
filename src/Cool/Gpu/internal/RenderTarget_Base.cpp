#include "RenderTarget_Base.h"

#if defined(__COOL_APP_VULKAN)
#include "RenderTarget_ImplVulkan.h"
template class Cool::RenderTarget_Base<Cool::RenderTarget_ImplVulkan>;
#elif defined(__COOL_APP_OPENGL)
#include "RenderTarget_ImplOpenGL.h"
template class Cool::RenderTarget_Base<Cool::RenderTarget_ImplOpenGL>;
#endif

namespace Cool {

template<typename T>
void RenderTarget_Base<T>::resize_if_necessary()
{
    const auto target_size = [&]() {
        return _imgui_window_size ? *_imgui_window_size : ImageSize{};
    }();

    if (target_size.width() != width() || target_size.height() != height()) {
        _impl.resize(target_size.width(), target_size.height());
    }
}

template<typename T>
void RenderTarget_Base<T>::imgui_window() const
{
    ImGui::Begin("MyImage", nullptr, ImGuiWindowFlags_NoScrollbar);
    auto size = ImGui::GetContentRegionAvail();
    if (size.x >= 1.f && size.y >= 1.f) {
        _imgui_window_size.emplace();
        _imgui_window_size->set_width(static_cast<ImageSize::DataType>(size.x));
        _imgui_window_size->set_height(static_cast<ImageSize::DataType>(size.y));
    }
    else {
        _imgui_window_size = std::nullopt;
    }
    ImGui::Image(
        _impl.imgui_texture_id(),
        {static_cast<float>(width()),
         static_cast<float>(height())},
        {0.f, 1.f}, {1.f, 0.f});
    ImGui::End();
}

} // namespace Cool