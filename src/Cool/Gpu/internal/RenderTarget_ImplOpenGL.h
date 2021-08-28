#pragma once
#if defined(__COOL_APP_OPENGL)

#include "../OpenGL/TextureFB.h"
#include "../RenderTargetInfo.h"

namespace Cool {

class RenderTarget_ImplOpenGL {
public:
    using RenderFuncType = std::function<void()>;
    void render(RenderFuncType render_fn);

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

} // namespace Cool

#endif