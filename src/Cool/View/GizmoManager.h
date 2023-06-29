#pragma once
#include <reg/src/AnyId.hpp>
#include <reg/src/Id.hpp>
#include "Cool/Input/MouseDragEvents.h"
#include "Cool/View/Gizmos.h"
#include "Gizmos.h"

namespace Cool {

class View;

class GizmoManager {
public:
    void               push(Gizmo_Point2D);
    void               on_frame_end();
    void               render(View const&);
    [[nodiscard]] auto is_dragging_gizmo() const -> bool;
    auto               on_drag_start(MouseDragStartEvent<ViewCoordinates> const&, View const&) -> bool;
    void               on_drag_update(MouseDragUpdateEvent<ViewCoordinates> const&);
    void               on_drag_stop(MouseDragStopEvent<ViewCoordinates> const&);

private:
    /// Calls the given callback on the dragged gizmo. Does nothing if no gizmo is being dragged.
    void               with_dragged_gizmo(std::function<void(Gizmo_Point2D const&)> const&);
    [[nodiscard]] auto hovered_gizmo(View const& view) const -> Gizmo_Point2D const*;

private:
    std::vector<Gizmo_Point2D> _gizmos{};
    reg::AnyId                 _dragged_gizmo_id{};
};

} // namespace Cool