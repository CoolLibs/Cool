#include "Projection_Perspective.h"

namespace Cool {

auto projection_matrix(const Projection_Perspective& proj, float aspect_ratio) -> glm::mat4
{
    return glm::perspective(proj.field_of_view_in_radians,
                            aspect_ratio,
                            proj.near_plane,
                            proj.far_plane);
}

auto imgui(Projection_Perspective& proj) -> bool
{
    bool b = false;
    ImGui::BeginGroup();
    b |= ImGui::SliderAngle("Field of View", &proj.field_of_view_in_radians, 0.001f, 180.f);
    b |= ImGui::SliderFloat("Near Plane", &proj.near_plane, 0.001f, 1.f);
    b |= ImGui::SliderFloat("Far Plane", &proj.far_plane, proj.near_plane + 0.001f, 500.f);
    ImGui::EndGroup();
    return b;
}

} // namespace Cool
