#include "ProjectionController_Perspective.h"
#include "Camera.h"

namespace Cool {

void ProjectionController_Perspective::apply_to(Camera& camera, float aspect_ratio) const
{
    camera.set_projection_matrix(
        glm::infinitePerspective(
            glm::degrees(_field_of_view_in_radians),
            aspect_ratio,
            _near_plane));
}

bool ProjectionController_Perspective::ImGui()
{
    bool b = false;
    b |= ImGui::SliderAngle("Field of View", &_field_of_view_in_radians, 0.001f, 180.f);
    b |= ImGui::SliderFloat("Near Plane", &_near_plane, 0.001f, 1.f);
    return b;
}

} // namespace Cool