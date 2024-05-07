#pragma once
#include "Cool/Gpu/Texture.h"
#include "Cool/Video/VideoDescriptor.h"

namespace Cool {

class TextureSource_VideoFile {
public:
    auto               imgui_widget() -> bool;
    [[nodiscard]] auto get_texture() const -> Texture const*;
    [[nodiscard]] auto get_error() const -> std::optional<std::string>;

    friend auto operator==(TextureSource_VideoFile const&, TextureSource_VideoFile const&) -> bool = default;

private:
    static uint32_t next_id;
    VideoDescriptor _video_descriptor{}; // TODO(Video) Ship a default video with Coollab? A short and lightweight one
    uint32_t        _id{next_id++};

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Video", _video_descriptor)
        );
    }
};

} // namespace Cool