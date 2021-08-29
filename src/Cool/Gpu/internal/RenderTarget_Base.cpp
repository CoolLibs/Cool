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
    const auto size = compute_size();
    if (_impl.size() != size) {
        _impl.resize(size);
    }
}

template<typename T>
ImageSize RenderTarget_Base<T>::compute_size() const
{
    return _imposed_size        ? *_imposed_size
           : _imgui_window_size ? *_imgui_window_size
                                : ImageSize{};
}

template<typename T>
void RenderTarget_Base<T>::imgui_window() const
{
    ImGui::Begin("MyImage", nullptr, ImGuiWindowFlags_NoScrollbar);
    auto size = ImGui::GetContentRegionAvail();
    if (size.x >= 1.f && size.y >= 1.f) {
        _imgui_window_size.emplace(
            static_cast<ImageSize::DataType>(size.x),
            static_cast<ImageSize::DataType>(size.y));
    }
    else {
        _imgui_window_size.reset();
    }
    const auto image_size = ImageSizeU::fit_into(_imgui_window_size.value_or(ImageSize{}), _impl.size());
    ImGui::Image(
        _impl.imgui_texture_id(),
        {image_size.width(),
         image_size.height()},
        {0.f, 1.f}, {1.f, 0.f});
    ImGui::End();
}

} // namespace Cool