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
            cereal::make_nvp("Has min bound", has_min_bound),
            cereal::make_nvp("Min", min),
            cereal::make_nvp("Has max bound", has_max_bound),
            cereal::make_nvp("Max", max),
            cereal::make_nvp("Drag speed", drag_speed),
            cereal::make_nvp("Use slider", use_slider),
            cereal::make_nvp("Is logarithmic", is_logarithmic)
        );
    }
};

} // namespace Cool::internal