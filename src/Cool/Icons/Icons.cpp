#include "Icons.h"
#include <Cool/File/File.h>
#include "img/src/Load.h"
#if DEBUG
#include <Cool/DebugOptions/DebugOptions.h>
#include <Cool/Log/ToUser.h>
#endif

namespace Cool {

std::map<std::filesystem::path, Texture> Icons::_map;

const Texture& Icons::get(std::filesystem::path image_path)
{
    const auto path = std::filesystem::absolute(image_path);
    auto       res  = _map.find(path);
    if (res == _map.end())
    {
#if DEBUG
        if (DebugOptions::log_when_creating_icon())
        {
            Log::Debug::info("Icons", fmt::format("Generating texture for \"{}\"", path));
        }
#endif
        _map[path] = Texture{img::load(path)};
        return _map[path];
    }
    else
    {
        return res->second;
    }
}

void Icons::cleanup_texture(std::filesystem::path image_path)
{
    const auto path = std::filesystem::absolute(image_path);
    auto       res  = _map.find(path);
    if (res == _map.end())
    {
        Log::Debug::warning(
            "Icons::cleanup_texture",
            fmt::format("The texture you want to clean up doesn't exist! \"{}\"", path)
        );
    }
    else
    {
        _map.erase(path);
    }
}

void Icons::shut_down()
{
    _map.clear();
}

} // namespace Cool