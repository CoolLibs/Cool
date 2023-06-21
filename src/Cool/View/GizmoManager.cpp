#include "GizmoManager.h"
#include <imgui.h>
#include <imgui_internal.h>
#include "Cool/Geometry/imvec_glm_conversions.h"
#include "View.h"

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

auto GizmoManager::is_dragging_gizmo() const -> bool
{
    return !_dragged_gizmo_id.underlying_uuid().is_nil();
}

void GizmoManager::on_drag_start(MouseDragStartEvent<ViewCoordinates> const& event)
{
    for (auto const& gizmo : _gizmos)
    {
        if (glm::length(gizmo.position - event.position) < 0.1f)
        {
            _dragged_gizmo_id = gizmo.id();
            return;
        }
    }
}

void GizmoManager::on_drag_update(MouseDragUpdateEvent<ViewCoordinates> const&)
{
    if (!is_dragging_gizmo())
        return;

    for (auto& gizmo : _gizmos)
    {
        if (gizmo.id() != _dragged_gizmo_id)
            continue;
    }
}

void GizmoManager::on_drag_stop(MouseDragStopEvent<ViewCoordinates> const&)
{
    _dragged_gizmo_id = {};
}

} // namespace Cool