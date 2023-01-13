#include "Camera2D.h"
#include <glm/gtx/matrix_transform_2d.hpp>
#include <stringify/stringify.hpp>
#include "Cool/ImGui/ImGuiExtras.h"
#include "Cool/StrongTypes/Angle.h"
#include "Cool/StrongTypes/Camera2D.h"
#include "glm/detail/qualifier.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"

namespace Cool {

auto Camera2D::transform_matrix() const -> glm::mat3
{
    glm::mat3 res{1.};

    res *= glm::translate(res, this->translation);
    res *= glm::rotate(res, this->rotate.as_radians());
    res *= glm::scale(res, {this->zoom, this->zoom});

    return {res};
}

auto to_string(Camera2D const&) -> std::string
{
    // je veux return translation et zoom
    return {}; // Cool::stringify(UNTRUC);
}

auto imgui_widget(std::string_view name, Camera2D& cam, int number_of_snaps, float snaps_offset, bool always_snap) -> bool
{
    bool b = false;
    ImGui::TextUnformatted(name.data());
    ImGui::BeginGroup();
    ImGui::PushID(name.data());
    b |= ImGui::DragFloat2("Translation", glm::value_ptr(cam.translation));
    b |= imgui_widget("Rotation", cam.rotate, number_of_snaps, snaps_offset, always_snap);
    b |= ImGui::DragFloat("Zoom", &cam.zoom);
    ImGui::PopID();
    ImGui::EndGroup();

    return b;
}

} // namespace Cool