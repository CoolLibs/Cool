#include "TextureLibrary.h"
#include "Cool/Gpu/Texture.h"
#include "Cool/Gpu/TextureInfo.h"

namespace Cool {

static auto gen_dummy_texture() -> Texture
{
    return Texture{img::Size{1, 1}, 3, std::array<uint8_t, 3>{255, 0, 255}.data()};
}

auto TextureLibrary::get(std::filesystem::path const& path) -> Texture const&
{
    auto const it = _textures.find(path);
    if (it != _textures.end())
        return it->second;

    try
    {
        _textures[path] = Texture{img::load(path)};
        return _textures[path];
    }
    catch (std::exception const& e)
    {
        // TODO(JF) Error message
        static auto const dummy_texture = gen_dummy_texture();
        return dummy_texture;
    }
}

} // namespace Cool