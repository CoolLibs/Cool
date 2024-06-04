#pragma once
#include "Cool/Time/Time.hpp"
#include "Cool/Time/TimeSpeed.h"

namespace Cool {

enum class VideoPlayerLoopMode {
    None,
    Loop,
    Hold,
    // TODO(Video) Mode Bounce (like mirror repeat for textures). But requires us to support playing videos backward
};

struct VideoPlayerSettings {
    VideoPlayerLoopMode loop_mode{VideoPlayerLoopMode::Loop};
    TimeSpeed           playback_speed{1.};
    Time                start_time{0s};

    friend auto operator==(VideoPlayerSettings const&, VideoPlayerSettings const&) -> bool = default;
    auto        imgui_widget() -> bool;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Loop mode", loop_mode),
            cereal::make_nvp("Playback speed", playback_speed),
            cereal::make_nvp("Start time", start_time)
        );
    }
};

struct VideoDescriptor {
    std::filesystem::path path{};
    VideoPlayerSettings   settings{};

    auto imgui_widget() -> bool;

    friend auto operator==(VideoDescriptor const&, VideoDescriptor const&) -> bool = default;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("File Path", path),
            cereal::make_nvp("Settings", settings)
        );
    }
};

} // namespace Cool