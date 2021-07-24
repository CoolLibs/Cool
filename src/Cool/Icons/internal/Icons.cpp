#include "../Icons.h"
// #include <Cool/GPU/Texture.h>
#include <Cool/LoadImage/LoadImage.h>
#include <filesystem>

namespace Cool {

std::unordered_map<std::string, Texture::Id> Icons::_map;

Texture::Id Icons::get(std::string_view image_path)
{
    const auto path = File::absolute_path(image_path);
    auto       res  = _map.find(path);
    if (res == _map.end()) {
        Log::info("[Icons::get] Generating texture for \"{}\"", path);
        const auto tex_id = Texture::LoadTexture(path);
        _map[path]        = tex_id;
        return tex_id;
    }
    else {
        return res->second;
    }
}

void Icons::cleanup_texture(std::string_view image_path)
{
    const auto path = File::absolute_path(image_path);
    auto       res  = _map.find(path);
    if (res == _map.end()) {
        Log::warn("[Icons::cleanup_texture] The texture you want to clean up doesn't exist ! \"{}\"", path);
    }
    else {
        // Texture::DestroyTexture(res->second);
        // _map.erase(path);
    }
}

} // namespace Cool