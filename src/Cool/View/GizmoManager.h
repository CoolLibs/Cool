#pragma once
#include "Gizmos.h"
#include "View.h"

namespace Cool {

class GizmoManager {
public:
    void push(Gizmo_Point2D);
    void on_frame_end();
    void render(View const&);

private:
    std::vector<Gizmo_Point2D> _gizmos{};
};

} // namespace Cool