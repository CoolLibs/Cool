#include "../Icons.h"
#include <filesystem>

namespace Cool {

std::unordered_map<std::string, Texture> Icons::_map;

Texture::Id Icons::get(std::string_view image_path)
{
    const auto path = File::absolute_path(image_path);
    auto       res  = _map.find(path);
    if (res == _map.end()) {
        Log::info("[Icons::get] Generating texture for \"{}\"", path);
        _map[path] = Texture{path};
        return _map[path].id();
    }
    else {
        return res->second.id();
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
        _map.erase(path);
    }
}

} // namespace Cool