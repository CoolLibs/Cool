#include "GizmoManager.h"
#include <imgui.h>
#include <imgui_internal.h>
#include "Cool/Geometry/imvec_glm_conversions.h"
#include "Cool/Input/MouseCoordinates.h"
#include "Cool/View/Gizmos.h"
#include "View.h"

namespace Cool {

void GizmoManager::push(Gizmo_Point2D gizmo)
{
    _gizmos.emplace_back(std::move(gizmo));
}

void GizmoManager::on_frame_end()
{
    _gizmos.clear();
}

static constexpr auto point2D_gizmo_radius = 10.f; // In ImGui coordinates

void GizmoManager::render(View const& view)
{
    if (is_dragging_gizmo()
        || hovered_gizmo(view))
    {
        ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeAll);
    }

    for (auto const& gizmo : _gizmos)
    {
        auto const radius    = point2D_gizmo_radius;
        auto const thickness = 4.f;
        ImGui::GetCurrentWindow()->DrawList->AddCircle(
            as_imvec(view.to_imgui_coordinates(gizmo.get_position())),
            radius - thickness / 4.f,
            ImColor{1.f, 1.f, 1.f, 1.f},
            0,
            thickness / 2.f
        );
        ImGui::GetCurrentWindow()->DrawList->AddCircle(
            as_imvec(view.to_imgui_coordinates(gizmo.get_position())),
            radius - thickness * 3 / 4.f,
            ImColor{0.f, 0.f, 0.f, 1.f},
            0,
            thickness / 2.f
        );
    }
}

auto GizmoManager::is_dragging_gizmo() const -> bool
{
    return !_dragged_gizmo_id.underlying_uuid().is_nil();
}

auto GizmoManager::on_drag_start(MouseDragStartEvent<ViewCoordinates> const&, View const& view) -> bool
{
    assert(!is_dragging_gizmo());
    auto const* gizmo = hovered_gizmo(view);
    if (!gizmo)
        return false;
    _dragged_gizmo_id = gizmo->id;
    return true;
}

void GizmoManager::on_drag_update(MouseDragUpdateEvent<ViewCoordinates> const& event)
{
    with_dragged_gizmo([&](Gizmo_Point2D const& gizmo) {
        gizmo.set_position(ViewCoordinates{gizmo.get_position() + event.delta});
    });
}

void GizmoManager::on_drag_stop(MouseDragStopEvent<ViewCoordinates> const&)
{
    with_dragged_gizmo([](Gizmo_Point2D const& gizmo) {
        gizmo.on_drag_stop();
    });
    _dragged_gizmo_id = {};
}

void GizmoManager::with_dragged_gizmo(std::function<void(Gizmo_Point2D const&)> const& callback)
{
    if (!is_dragging_gizmo())
        return;

    for (auto const& gizmo : _gizmos)
    {
        if (gizmo.id != _dragged_gizmo_id)
            continue;
        callback(gizmo);
    }
}

auto GizmoManager::hovered_gizmo(View const& view) const -> Gizmo_Point2D const*
{
    auto const radius = 7.5f + point2D_gizmo_radius;
    for (auto const& gizmo : _gizmos)
    {
        if (glm::length(view.to_imgui_coordinates(gizmo.get_position()) - as_glm(ImGui::GetMousePos())) < radius)
        {
            return &gizmo;
        }
    }
    return nullptr;
}

} // namespace Cool