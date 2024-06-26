#pragma once
#include <chrono>
#include <filesystem>
#include <map>
#include <optional>
#include "Cool/Gpu/Texture.h"
#include "Cool/Video/VideoDescriptor.h"
#include "Cool/Video/VideoPlayer.h"

namespace Cool {

struct VideoRequest {
    uint32_t        id{};
    VideoDescriptor video_descriptor{};
};

class TextureLibrary_Video {
public:
    [[nodiscard]] static auto instance() -> TextureLibrary_Video&
    {
        static auto inst = TextureLibrary_Video{};
        return inst;
    }

    [[nodiscard]] auto get(VideoRequest const&) -> Texture const*;

    void update();
    auto error_from(VideoRequest const&) -> std::optional<std::string>;
    auto detailed_video_info(std::filesystem::path const&) const -> std::string const*;

    void imgui_debug_view() const;

private:
    TextureLibrary_Video() = default; // This is a singleton. Get the global instance with `instance()` instead.

private:
    using clock = std::chrono::steady_clock;
    struct Data {
        VideoPlayer       player;
        clock::time_point date_of_last_request{};
    };
    struct Key { // ID of the request + path. i.e. we recreate a player only if the path changes, or if a request comes from a new texture source. When the settings change (i.e. loop mode) we will just modify the current player, to avoid the costly recreation of a player.
        uint32_t              id;
        std::filesystem::path path;

        friend auto operator<(Key const& a, Key const& b) -> bool { return a.id < b.id || (a.id == b.id && a.path < b.path); }
    };
    std::map<Key, Data> _videos;
};

} // namespace Cool