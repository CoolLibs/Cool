#pragma once
#if defined(COOL_OPENGL)

#include <Cool/Image/ImageData.h>
#include <Cool/Image/ImageSize.h>
#include "../OpenGL/TextureFB.h"
#include "../RenderTargetInfo.h"

namespace Cool {

class RenderTarget_ImplOpenGL {
public:
    using RenderFuncType = std::function<void()>;
    RenderTarget_ImplOpenGL(ImageSize size = {});
    void render(RenderFuncType render_fn);

    RenderTargetInfo    info() const;
    ImageSize::DataType width() const { return _texture.width(); }
    ImageSize::DataType height() const { return _texture.height(); }
    ImageSize           size() const { return _texture.size(); }
    ImageData           download_pixels() const;
    // const Texture&   texture() const { return _texture.; }
    ImTextureID imgui_texture_id() const { return reinterpret_cast<ImTextureID>(static_cast<uint64_t>(_texture.textureID())); } // Double-cast to fix a warning : first we convert to the correct size (uint32_t -> uint64_t) then from integral type to pointer type (uint64_t -> ImTextureID)

    void imgui_window();

    void resize(ImageSize size);

private:
    TextureFB _texture;
};

} // namespace Cool

#endif