#pragma once
#include "VideoPlayerSettings.hpp"

namespace Cool {

struct VideoDescriptor {
    std::filesystem::path path{};
    VideoPlayerSettings   settings{};

    auto imgui_widget() -> bool;

    friend auto operator==(VideoDescriptor const&, VideoDescriptor const&) -> bool = default;

private:
    // Serialization
    friend class ser20::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            ser20::make_nvp("File Path", path),
            ser20::make_nvp("Settings", settings)
        );
    }
};

} // namespace Cool