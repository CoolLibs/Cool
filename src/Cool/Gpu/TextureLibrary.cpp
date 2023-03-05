#include "TextureLibrary.h"
#include <filesystem>
#include "Cool/DebugOptions/DebugOptions.h"
#include "Cool/FileWatcher/FileWatcher.h"
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
        return it->second.first;

    try
    {
        _textures[path] = std::make_pair(Texture{img::load(path)}, FileWatcher{path, FileWatcher_NoCallbacks()});
        if (DebugOptions::log_when_creating_textures())
            Log::ToUser::info("TextureLibrary", fmt::format("Generated texture from {}", path));
        return _textures[path].first;
    }
    catch (std::exception const& e)
    {
        static auto const dummy_texture = gen_dummy_texture();
        return dummy_texture;
    }
}

auto TextureLibrary::update() -> bool
{
    // We remove the textures that have changed so that they will be reloaded the next time we need them.
    auto       to_remove            = std::vector<std::filesystem::path>{};
    auto const store_path_to_remove = [&](std::filesystem::path const& path) {
        to_remove.push_back(path);
    };
    for (auto const& kv : _textures)
        kv.second.second.update({.on_file_changed = store_path_to_remove, .on_path_invalid = store_path_to_remove});

    for (auto const& path : to_remove)
        _textures.erase(path);

    return !to_remove.empty();
}

} // namespace Cool