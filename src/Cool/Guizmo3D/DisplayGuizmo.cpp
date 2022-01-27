#include "DisplayGuizmo.h"

namespace Cool {
bool display_guizmo(CameraManager camera)
{
    ImGuizmo::SetOrthographic(false);
    ImGuizmo::SetDrawlist();

    bool has_guizmo = false;

    float windowHeight = (float)ImGui::GetWindowHeight();
    float windowWidth  = (float)ImGui::GetWindowWidth();
    ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

    auto& selected_nodes = Cool::GGS;

    for (auto& guizmo_state : selected_nodes) {
        if (ImGuizmo::Manipulate(glm::value_ptr(camera->view_matrix()), glm::value_ptr(camera->projection_matrix()),
                                 guizmo_state.second.operation, ImGuizmo::LOCAL, glm::value_ptr(guizmo_state.second.transform_matrix.get()))) {
            guizmo_state.second.has_changed = true;
            has_guizmo                      = true;
        };
    }
    return has_guizmo;
}
} // namespace Cool