#pragma once
#include "Cool/StrongTypes/Angle.h"

namespace Cool {

struct Camera2D {
    glm::vec2 translation{0.f, 0.f};
    Angle     rotation{Radians(0.f)};
    float     zoom{1.f};

    auto transform_matrix(float aspect_ratio) const -> glm::mat3;

    friend auto operator==(Camera2D const& a, Camera2D const& b) -> bool = default;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Translation", translation),
            cereal::make_nvp("Zoom", zoom),
            cereal::make_nvp("Rotation", rotation)
        );
    }
};

auto to_string(Camera2D const&) -> std::string;
auto imgui_widget(std::string_view name, Camera2D& cam, int number_of_snaps, float snaps_offset, bool always_snap) -> bool;

} // namespace Cool