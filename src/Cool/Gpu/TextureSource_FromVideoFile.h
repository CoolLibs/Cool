#pragma once
#include "Cool/Gpu/Texture.h"

namespace Cool {

struct TextureSource_FromVideoFile {
    std::filesystem::path path{}; // TODO(Video) Ship a default video with Coollab? A short and lightweight one
    bool                  do_loop{true};
    float                 playback_speed{1.f};
    float                 time_offset{0.f};

    auto               imgui_widget() -> bool;
    [[nodiscard]] auto get_texture() const -> Texture const*;
    [[nodiscard]] auto get_error() const -> std::optional<std::string>;

    friend auto operator==(TextureSource_FromVideoFile const&, TextureSource_FromVideoFile const&) -> bool = default;

private:
    int64_t _current_time_in_nanoseconds{0};

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("File path", path),
            cereal::make_nvp("Does loop", do_loop),
            cereal::make_nvp("Playback speed", playback_speed),
            cereal::make_nvp("Time offset", time_offset)
        );
    }
};

} // namespace Cool