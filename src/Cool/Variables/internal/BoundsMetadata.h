#pragma once

namespace Cool::internal {

template<typename T>
struct BoundsMetadata {
    T     min{};
    T     max{};
    bool  has_min_bound{false};
    bool  has_max_bound{false};
    float drag_speed{0.01f};
    bool  use_slider{false};
    bool  is_logarithmic{false};

    friend auto operator<=>(BoundsMetadata const&, BoundsMetadata const&) = default;

    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            ser20::make_nvp("Has min bound", has_min_bound),
            ser20::make_nvp("Min", min),
            ser20::make_nvp("Has max bound", has_max_bound),
            ser20::make_nvp("Max", max),
            ser20::make_nvp("Drag speed", drag_speed),
            ser20::make_nvp("Use slider", use_slider),
            ser20::make_nvp("Is logarithmic", is_logarithmic)
        );
    }
};

} // namespace Cool::internal