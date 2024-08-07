#pragma once

namespace Cool {

struct Projection_Perspective {
    float field_of_view_in_radians = glm::pi<float>() * 0.5f;
    float near_plane               = 0.1f;
    float far_plane                = 200.f;

    auto operator==(const Projection_Perspective&) const -> bool = default;

private:
    // Serialization
    friend class ser20::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            ser20::make_nvp("Field of View (in radians)", field_of_view_in_radians),
            ser20::make_nvp("Near Plane", near_plane),
            ser20::make_nvp("Far Plane", far_plane)
        );
    }
};

auto projection_matrix(const Projection_Perspective& proj, float aspect_ratio) -> glm::mat4;
auto imgui(Projection_Perspective&) -> bool;

} // namespace Cool
