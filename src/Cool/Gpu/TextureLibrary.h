#pragma once
#include <map>
#include "Texture.h"
#include "TextureInfo.h"

namespace Cool {

/// Compared to the Icons class, this one does not assume that the
/// images will be found nor that they will stay the same forever.
/// This is why it adds checks to see if the texture exists, and
/// re-loads it if the source file changes.
class TextureLibrary {
public:
    /// Might return nullptr if the file does not exist or is not an image.
    [[nodiscard]] auto get(std::filesystem::path const&) -> Texture const&;

    void clear() { _textures.clear(); }

    [[nodiscard]] static auto instance() -> TextureLibrary&
    {
        static auto inst = TextureLibrary{};
        return inst;
    }

private:
    std::map<std::filesystem::path, Texture> _textures;
    // TODO(JF) File watchers for all the textures we are currently storing
};

} // namespace Cool