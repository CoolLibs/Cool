#pragma once
#include <chrono>
#include <map>
#include <optional>
#include "Cool/FileWatcher/FileWatcher.h"
#include "Cool/Gpu/Texture.h"
#include "TextureDescriptor.h"

namespace Cool {

/// Compared to the Icons class, this one does not assume that the
/// images will be found nor that they will stay the same forever.
/// This is why it adds checks to see if the texture exists, and
/// re-loads it if the source file changes.
class TextureLibrary_Image {
public:
    [[nodiscard]] static auto instance() -> TextureLibrary_Image&
    {
        static auto inst = TextureLibrary_Image{};
        return inst;
    }

    [[nodiscard]] auto get(std::filesystem::path const&) -> Texture const*;

    /// Returns true iff at least one of the textures in the library has changed.
    auto update() -> bool;
    auto error_from(std::filesystem::path const&) -> std::optional<std::string>;

    void imgui_debug_view() const;

private:
    TextureLibrary_Image() = default; // This is a singleton. Get the global instance with `instance()` instead.
    void reload_texture(std::filesystem::path const& path);

private:
    using clock = std::chrono::steady_clock;
    struct Data {
        std::optional<Texture>     texture{};
        FileWatcher                file_watcher{};
        clock::time_point          date_of_last_request{};
        std::optional<std::string> error_message{};
    };
    std::map<std::filesystem::path, Data> _textures;
};

} // namespace Cool