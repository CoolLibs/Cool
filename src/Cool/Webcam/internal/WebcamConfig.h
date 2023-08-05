#pragma once

namespace Cool {

struct WebcamConfig {
    webcam_info::Resolution resolution;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Width", resolution.width),
            cereal::make_nvp("Height", resolution.height)
        );
    }
};

using WebcamsConfigsList = std::unordered_map<std::string, WebcamConfig>;

} // namespace Cool