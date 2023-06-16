#pragma once
#include "Gizmos.h"

namespace Cool {

class GizmoManager {
public:
    void push(Gizmo_Point2D);
    void on_frame_end();
    void render();

private:
    std::vector<Gizmo_Point2D> _gizmos{};
};

} // namespace Cool