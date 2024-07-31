#pragma once
#include "Cool/Time/Time.hpp"

namespace Cool {

struct VideoExportParams {
    double fps       = 30.;
    Time   beginning = 0s;
    Time   end       = 10s;

    void imgui();

private:
    // Serialization
    friend class ser20::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            ser20::make_nvp("FPS", fps),
            ser20::make_nvp("Beginning", beginning),
            ser20::make_nvp("End", end)
        );
    }
};

} // namespace Cool