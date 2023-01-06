#pragma once

namespace Cool::internal {

template<typename T>
struct BoundsMetadata {
    T     min{};
    T     max{};
    float drag_speed{0.01f};
    bool  is_bounded{false};

    friend auto operator==(BoundsMetadata const&, BoundsMetadata const&) -> bool = default;

    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Min", min),
            cereal::make_nvp("Max", max),
            cereal::make_nvp("Drag speed", drag_speed),
            cereal::make_nvp("Is bounded", is_bounded)
        );
    }
};

} // namespace Cool::internal