#include "Icons.h"
#include "Cool/DebugOptions/DebugOptions.h"
#include "Cool/File/File.h"
#include "Cool/TextureSource/default_textures.h"
#include "img/src/Load.h"

namespace Cool {

std::map<std::filesystem::path, Texture> Icons::_map;

const Texture& Icons::get(std::filesystem::path image_path)
{
    const auto path = File::make_absolute(image_path);
    auto       res  = _map.find(path);
    if (res == _map.end())
    {
        auto const image = img::load(path);
        _map[path]       = Texture{image.has_value() ? *image : dummy_image()};
        if (DebugOptions::log_when_creating_textures())
            Log::info("Icons", fmt::format("Generated texture from \"{}\"", Cool::File::weakly_canonical(path)));
        return _map[path];
    }
    else
    {
        return res->second;
    }
}

void Icons::cleanup_texture(std::filesystem::path const& image_path)
{
    const auto path = File::make_absolute(image_path);
    auto       res  = _map.find(path);
    if (res == _map.end())
    {
        Log::internal_warning("Icons::cleanup_texture", fmt::format("The texture you want to clean up doesn't exist! \"{}\"", Cool::File::weakly_canonical(path)));
        return;
    }
    _map.erase(path);
}

} // namespace Cool