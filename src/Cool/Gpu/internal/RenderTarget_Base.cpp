#include "RenderTarget_Base.h"
#include <Cool/ImGuiExtras/ImGuiExtras.h>

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
void RenderTarget_Base<T>::imgui_window(std::string_view name) const
{
    ImGui::Begin(name.data(), nullptr, ImGuiWindowFlags_NoScrollbar);
    // Update _imgui_window_size
    auto size = ImGui::GetContentRegionAvail();
    if (size.x >= 1.f && size.y >= 1.f) {
        _imgui_window_size.emplace(
            static_cast<ImageSize::DataType>(size.x),
            static_cast<ImageSize::DataType>(size.y));
    }
    else {
        _imgui_window_size.reset();
    }
    // Display the image
    const auto window_size = _imgui_window_size.value_or(ImageSize{});
    const auto image_size  = _imposed_size.has_value()
                                 ? ImageSizeU::fit_into(window_size, _impl.size())
                                 : static_cast<ImageSizeT<float>>(window_size);
    ImGuiExtras::image_centered(
        _impl.imgui_texture_id(),
        {image_size.width(),
         image_size.height()});
    ImGui::End();
}

} // namespace Cool