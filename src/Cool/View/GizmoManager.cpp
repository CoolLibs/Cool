#include "GizmoManager.h"
#include <imgui.h>
#include <imgui_internal.h>

namespace Cool {

void GizmoManager::push(Gizmo_Point2D gizmo)
{
    _gizmos.push_back(gizmo);
}

void GizmoManager::on_frame_end()
{
    _gizmos.clear();
}

void GizmoManager::render()
{
    for (auto const& gizmo : _gizmos)
    {
        ImGui::GetCurrentWindow()->DrawList->AddCircleFilled({gizmo.position.x, gizmo.position.y}, 500.f, ImColor{1.f, 1.f, 1.f, 1.f});
    }
}

} // namespace Cool