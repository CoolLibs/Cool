#pragma once
#include "Cool/StrongTypes/Angle.h"

namespace Cool {

struct Camera2D {
    glm::vec2 translation{0.f, 0.f};
    Angle     rotation{Radians(0.f)};
    float     zoom{1.f};

    /// Inverse of view_matrix().
    auto transform_matrix() const -> glm::mat3;
    /// Inverse of transform_matrix().
    auto view_matrix() const -> glm::mat3;
    auto view_projection_matrix(float inverse_aspect_ratio) const -> glm::mat3;

    friend auto operator==(Camera2D const& a, Camera2D const& b) -> bool = default;

private:
    // Serialization
    friend class ser20::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            ser20::make_nvp("Translation", translation),
            ser20::make_nvp("Zoom", zoom),
            ser20::make_nvp("Rotation", rotation)
        );
    }
};

auto to_string(Camera2D const&) -> std::string;
auto imgui_widget(std::string_view name, Camera2D& cam, int number_of_snaps, float snaps_offset, bool always_snap) -> bool;

} // namespace Cool