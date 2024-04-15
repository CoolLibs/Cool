#pragma once
#include "Cool/Gpu/Texture.h"
#include "Cool/Video/VideoPlayer.h"

namespace Cool {

class TextureSource_FromVideoFile {
public:
    auto               imgui_widget() -> bool;
    [[nodiscard]] auto get_texture() const -> Texture const*;
    [[nodiscard]] auto get_error() const -> std::optional<std::string>;

    friend auto operator==(TextureSource_FromVideoFile const& a, TextureSource_FromVideoFile const& b) -> bool
    {
        return *a._player == *b._player;
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
            cereal::make_nvp("Video Player", *_player)
        );
    }
};

} // namespace Cool