#include "Camera2D.h"
#include <Cool/Icons/Icons.h>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <stringify/stringify.hpp>
#include "Cool/ImGui/ImGuiExtras.h"
#include "Cool/StrongTypes/Angle.h"
#include "Cool/StrongTypes/Camera2D.h"
#include "imgui.h"

namespace Cool {

auto Camera2D::transform_matrix() const -> glm::mat3
{
    auto res = glm::mat3{1.f};

    res = glm::rotate(res, rotation.as_radians());
    res = glm::translate(res, translation);
    res = glm::scale(res, glm::vec2{1.f / zoom});

    return res;
}

auto to_string(Camera2D const& cam) -> std::string
{
    return fmt::format(
        "\nTranslation: {}\nRotation: {}\nZoom: {}",
        Cool::stringify(cam.translation),
        Cool::stringify(cam.rotation),
        Cool::stringify(cam.zoom)
    );
}

auto imgui_widget(std::string_view name, Camera2D& cam, int number_of_snaps, float snaps_offset, bool always_snap) -> bool
{
    bool b = false;

    ImGui::TextUnformatted(name.data());
    ImGui::BeginGroup();
    ImGui::PushID(name.data());

    b |= ImGui::DragFloat2("Translation", glm::value_ptr(cam.translation), 0.01f);
    b |= imgui_widget("Rotation", cam.rotation, number_of_snaps, snaps_offset, always_snap);
    b |= ImGui::DragFloat("Zoom", &cam.zoom, 0.01f, 0.001f, FLT_MAX / static_cast<float>(INT_MAX));
    if (ImGuiExtras::button_with_icon(Icons::reset().imgui_texture_id()))
    {
        cam.rotation    = Cool::Angle{Cool::Radians{0.}};
        cam.zoom        = 1.f;
        cam.translation = glm::vec2(0.);
        b               = true;
    }
    Cool::ImGuiExtras::tooltip("Reset Camera");

    ImGui::PopID();
    ImGui::EndGroup();

    return b;
}

} // namespace Cool