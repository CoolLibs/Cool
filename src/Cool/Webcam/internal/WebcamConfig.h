#pragma once
#include <webcam_info/webcam_info.hpp>

namespace Cool {

struct WebcamConfig {
    webcam_info::Resolution resolution;

private:
    // Serialization
    friend class ser20::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            ser20::make_nvp("Width", resolution.width),
            ser20::make_nvp("Height", resolution.height)
        );
    }
};

} // namespace Cool