#pragma once
#include <map>
#include "Cool/FileWatcher/FileWatcher.h"
#include "Texture.h"
#include "TextureInfo.h"

namespace Cool {

/// Compared to the Icons class, this one does not assume that the
/// images will be found nor that they will stay the same forever.
/// This is why it adds checks to see if the texture exists, and
/// re-loads it if the source file changes.
class TextureLibrary {
public:
    [[nodiscard]] auto get(std::filesystem::path const&) -> Texture const&;

    void clear() { _textures.clear(); }
    /// Returns true iff at least one of the textures in the library has changed.
    auto update() -> bool;

    [[nodiscard]] static auto instance() -> TextureLibrary&
    {
        static auto inst = TextureLibrary{};
        return inst;
    }

private:
    std::map<std::filesystem::path, std::pair<Texture, FileWatcher>> _textures;
};

} // namespace Cool