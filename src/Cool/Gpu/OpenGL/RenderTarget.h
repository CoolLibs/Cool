#pragma once
#if defined(__COOL_APP_OPENGL)

#include "../RenderTargetInfo.h"
#include "TextureFB.h"

namespace Cool::OpenGL {

class RenderTarget {
public:
    void render(std::function<void()> render_fn);

    RenderTargetInfo info() const;
    uint32_t         width() const { return _texture.width(); }
    uint32_t         height() const { return _texture.height(); }
    // const Texture&   texture() const { return _texture.; }
    ImTextureID imgui_texture_id() const { return reinterpret_cast<ImTextureID>(_texture.textureID()); }

    void imgui_window();

    void resize(uint32_t width, uint32_t height);

private:
    TextureFB _texture;
};

} // namespace Cool::OpenGL

#endif