#pragma once
#include "Cool/Time/Time.h"

namespace Cool {

struct VideoExportParams {
    double fps       = 30.;
    Time   beginning = 0s;
    Time   end       = 10s;

    void imgui();

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("FPS", fps),
            cereal::make_nvp("Beginning", beginning),
            cereal::make_nvp("End", end)
        );
    }
};

} // namespace Cool