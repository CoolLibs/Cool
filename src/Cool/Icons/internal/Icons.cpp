#include "../Icons.h"
#include <Cool/File/File.h>

namespace Cool {

std::unordered_map<std::string, Texture> Icons::_map;

const Texture& Icons::get(std::string_view image_path)
{
    const auto path = File::absolute_path(image_path);
    auto       res  = _map.find(path);
    if (res == _map.end()) {
        Log::info("[Icons::get] Generating texture for \"{}\"", path);
        _map[path] = Texture{path};
        return _map[path];
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
        _map.erase(path);
    }
}

void Icons::shut_down()
{
    _map.clear();
}

} // namespace Cool