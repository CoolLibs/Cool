#include "TextureWrapper.h"
#include "Cool/Gpu/Texture.h"
#include "Cool/ImGui/ImGuiExtras.h"
#include "Cool/NfdFileFilter/NfdFileFilter.h"

namespace Cool {

static auto gen_dummy_texture() -> Texture
{
    Texture tex{};
    tex.genTexture();
    std::array<unsigned char, 3> data = {255, 0, 255};
    tex.uploadRGB(1, 1, data.data());
    return tex;
}

[[nodiscard]] auto TextureWrapper::texture() const -> Texture const&
{
    static auto const dummy_texture = gen_dummy_texture();
    return _texture ? *_texture : dummy_texture;
}

auto TextureWrapper::imgui_widget() -> bool
{
    bool const b = ImGuiExtras::file_and_folder("Image Path", &_absolute_path, NfdFileFilter::Image);
    return b;
}

} // namespace Cool