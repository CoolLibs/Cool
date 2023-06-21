#include "GizmoManager.h"
#include <imgui.h>
#include <imgui_internal.h>
#include "Cool/Geometry/imvec_glm_conversions.h"

namespace Cool {

void GizmoManager::push(Gizmo_Point2D gizmo)
{
    _gizmos.push_back(gizmo);
}

void GizmoManager::on_frame_end()
{
    _gizmos.clear();
}

void GizmoManager::render(View const& view)
{
    for (auto const& gizmo : _gizmos)
    {
        ImGui::GetCurrentWindow()->DrawList->AddCircleFilled(as_imvec(view.to_imgui_coordinates(gizmo.position)), 10.f, ImColor{1.f, 1.f, 1.f, 1.f});
    }
}

} // namespace Cool