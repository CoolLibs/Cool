#pragma once
#include "Cool/Gpu/Texture.h"
#include "Cool/Video/VideoPlayer.h"

namespace Cool {

struct VideoPlayerSettings { // TODO(Video) Move to video folder
    bool  do_loop{true};
    float playback_speed{1.f};
    float time_offset{0.f};

    friend auto operator==(VideoPlayerSettings const&, VideoPlayerSettings const&) -> bool = default;
    auto        imgui_widget() -> bool;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Does loop", do_loop),
            cereal::make_nvp("Playback speed", playback_speed),
            cereal::make_nvp("Time offset", time_offset)
        );
    }
};

struct TextureSource_FromVideoFile {
    VideoPlayerSettings settings{};

    auto               imgui_widget() -> bool;
    [[nodiscard]] auto get_texture() const -> Texture const*;
    [[nodiscard]] auto get_error() const -> std::optional<std::string>;

    friend auto operator==(TextureSource_FromVideoFile const& a, TextureSource_FromVideoFile const& b) -> bool
    {
        return a.settings == b.settings
               && a._player->path() == b._player->path();
    }

private:
    std::shared_ptr<VideoPlayer> _player{std::make_shared<VideoPlayer>()}; // TODO(Video) Remove need for shared_ptr
                                                                           // TODO(Video) Ship a default video with Coollab? A short and lightweight one

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Video Player", *_player),
            cereal::make_nvp("Settings", settings)
        );
    }
};

} // namespace Cool