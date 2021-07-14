#pragma once

namespace Cool {

class Camera;

class ProjectionController_Perspective {
public:
    void         apply_to(Camera& camera) const;
    inline float focal_length() const { return 1.f / tan(_field_of_view_in_radians / 2.f); }
    bool         ImGui();

private:
    float _field_of_view_in_radians = glm::pi<float>() * 0.5f;
    float _near_plane               = 0.1f;
};

} // namespace Cool