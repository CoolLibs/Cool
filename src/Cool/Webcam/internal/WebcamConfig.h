#pragma once
#include <wcam/wcam.hpp>

namespace Cool {

struct WebcamConfig {
    wcam::Resolution resolution;

private:
    // Serialization
    friend class ser20::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            ser20::make_nvp("Width", resolution.width()),
            ser20::make_nvp("Height", resolution.height())
        );
    }
};

} // namespace Cool