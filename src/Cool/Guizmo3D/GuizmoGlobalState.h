#pragma once
#include <unordered_map>

namespace Cool {

struct GuizmoState {
    std::reference_wrapper<glm::mat4> transform_matrix;
    ImGuizmo::OPERATION               operation;
    bool                              has_changed = false;
};

extern std::unordered_map<void*, GuizmoState>
    GGS;
}; // namespace Cool
