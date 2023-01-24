#include "TextureWrapper.h"
#include <exception>
#include <memory>
#include "Cool/Gpu/Texture.h"
#include "Cool/ImGui/ImGuiExtras.h"
#include "Cool/Log/Message.h"
#include "Cool/Log/ToUser.h"
#include "Cool/NfdFileFilter/NfdFileFilter.h"
#include "Cool/StrongTypes/TextureWrapper.h"
#include "img/src/Image.h"
#include "img/src/Size.h"

namespace Cool {

static auto gen_dummy_texture() -> Texture
{
    return Texture{img::Image(img::Size{1, 1}, 3, std::array<uint8_t, 3>{255, 0, 255}.data())};
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
        _texture = std::make_shared<Texture>(img::load(_absolute_path));
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
    switch (_repeat_mode)
    {
    case TextureRepeatMode::None:
    {
        _texture->set_wrap_mode(glpp::Wrap::ClampToBorder);
        GLfloat color[4] = {0.f, 0.f, 0.f, 0.f};
        GLDebug(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color));
        break;
    }
    case TextureRepeatMode::Mirror:
    {
        _texture->set_wrap_mode(glpp::Wrap::MirroredRepeat);
        break;
    }
    case TextureRepeatMode::Mosaic:
    {
        _texture->set_wrap_mode(glpp::Wrap::Repeat);
        break;
    }
    case TextureRepeatMode::Clamp:
    {
        _texture->set_wrap_mode(glpp::Wrap::ClampToEdge);
        break;
    }
    }
}

} // namespace Cool