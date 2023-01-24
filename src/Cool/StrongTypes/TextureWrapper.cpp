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
    bool b = false;

    if (ImGuiExtras::file_and_folder("Image Path", &_absolute_path, NfdFileFilter::Image))
    {
        try_load_texture_from_path();
        b = true;
    }

    if (ImGui::Combo("Repeat Mode", reinterpret_cast<int*>(&_repeat_mode), "None\0Mirror\0Mosaic\0Clamp\0\0"))
    {
        if (_texture)
            apply_repeat_mode();
        b = true;
    }

    return b;
}

void TextureWrapper::try_load_texture_from_path()
{
    try
    {
        _texture = std::make_shared<Texture>(_absolute_path, GL_LINEAR);
        apply_repeat_mode();
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

void TextureWrapper::apply_repeat_mode()
{
    GLDebug(glBindTexture(GL_TEXTURE_2D, _texture->ID()));

    switch (_repeat_mode)
    {
    case TextureRepeatMode::None:
    {
        GLDebug(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
        GLDebug(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
        GLfloat color[4] = {0.f, 0.f, 0.f, 0.f};
        GLDebug(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color));
        break;
    }
    case TextureRepeatMode::Mirror:
    {
        GLDebug(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT));
        GLDebug(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT));
        break;
    }
    case TextureRepeatMode::Mosaic:
    {
        GLDebug(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
        GLDebug(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
        break;
    }
    case TextureRepeatMode::Clamp:
    {
        GLDebug(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GLDebug(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        break;
    }
    }

    GLDebug(glBindTexture(GL_TEXTURE_2D, 0));
}

} // namespace Cool