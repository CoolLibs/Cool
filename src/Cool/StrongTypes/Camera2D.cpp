#include "Camera2D.h"
#include <Cool/Icons/Icons.h>
#include <Cool/ImGui/icon_fmt.h>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <stringify/stringify.hpp>
#include "Cool/ImGui/IcoMoonCodepoints.h"
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

auto Camera2D::view_matrix() const -> glm::mat3
{
    auto res = glm::mat3{1.f};

    res = glm::scale(res, glm::vec2{zoom});
    res = glm::translate(res, -translation);
    res = glm::rotate(res, -rotation.as_radians());

    return res;
}

auto Camera2D::view_projection_matrix(float inverse_aspect_ratio) const -> glm::mat3
{
    return glm::scale(glm::mat3{1.f}, glm::vec2{inverse_aspect_ratio, 1.f}) * view_matrix();
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

    ImGui::BeginGroup();
    ImGui::PushID(name.data());

    b |= ImGui::DragFloat2("Translation", glm::value_ptr(cam.translation), 0.01f);
    b |= imgui_widget("Rotation", cam.rotation, number_of_snaps, snaps_offset, always_snap);
    b |= ImGui::DragFloat("Zoom", &cam.zoom, 0.01f, 0.001f, FLT_MAX, "%.3f", ImGuiSliderFlags_Logarithmic);
    if (ImGui::Button(icon_fmt("Reset Camera", ICOMOON_TARGET).c_str()))
    {
        cam = {};
        b   = true;
    }

    ImGui::PopID();
    ImGui::EndGroup();

    return b;
}

} // namespace Cool