#include "TextureWrapper.h"
#include <exception>
#include <memory>
#include "Cool/Gpu/Texture.h"
#include "Cool/ImGui/ImGuiExtras.h"
#include "Cool/Log/Message.h"
#include "Cool/Log/ToUser.h"
#include "Cool/NfdFileFilter/NfdFileFilter.h"
#include "Cool/StrongTypes/TextureWrapper.h"

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

    if (b)
        try_load_texture_from_path();

    return b;
}

void TextureWrapper::try_load_texture_from_path()
{
    try
    {
        _texture = std::make_shared<Texture>(_absolute_path);
        Cool::Log::ToUser::console().remove(_error_id);
    }
    catch (std::exception const& e)
    {
        _texture.reset();
        Cool::Log::ToUser::console().send(
            _error_id,
            Message{
                .category = "Load Image",
                .message  = fmt::format("Failed to load '{}':\n{}", _absolute_path, e.what()),
                .severity = MessageSeverity::Error,
            }
        );
    }
}

} // namespace Cool