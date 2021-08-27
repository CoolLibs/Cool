#if defined(__COOL_APP_OPENGL)

#include "../RenderTarget.h"

namespace Cool::OpenGL {

void RenderTarget::render(std::function<void()> render_fn)
{
    resize_if_necessary();
    _texture.bind();
    render_fn();
    _texture.unbind();
}

RenderTargetInfo RenderTarget::info() const
{
    RenderTargetInfo info;
    info.viewport = RectSizePos{
        static_cast<int>(width()),
        static_cast<int>(height())};
    return info;
}

void RenderTarget::resize(uint32_t width, uint32_t height)
{
    _texture.setSize({width, height});
}

void RenderTarget::resize_if_necessary()
{
    if (_imgui_window_size.x != width() || _imgui_window_size.y != height()) {
        resize(_imgui_window_size.x, _imgui_window_size.y);
    }
}

void RenderTarget::imgui_window()
{
    ImGui::Begin("MyImage", nullptr, ImGuiWindowFlags_NoScrollbar);
    auto size = ImGui::GetContentRegionAvail();
    if (size.x > 0.f && size.y > 0.f) {
        _imgui_window_size = {static_cast<uint32_t>(size.x), static_cast<uint32_t>(size.y)};
    }
    ImGui::Image(
        reinterpret_cast<ImTextureID>(_texture.textureID()),
        {static_cast<float>(width()),
         static_cast<float>(height())});
    ImGui::End();
}

} // namespace Cool::OpenGL

#endif