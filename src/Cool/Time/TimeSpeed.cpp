#include "TimeSpeed.h"
#include <imgui.h>

namespace Cool {

auto to_string(TimeSpeed time_speed) -> std::string
{
    return fmt::format("x{:.2f}", time_speed.value);
}

auto imgui_widget(const char* label, TimeSpeed* time_speed, float v_speed, double v_min, double v_max, ImGuiSliderFlags flags) -> bool
{
    return ImGui::DragScalar(label, ImGuiDataType_Double, &time_speed->value, v_speed, &v_min, &v_max, "x%.2f", flags | ImGuiSliderFlags_NoRoundToFormat | ImGuiSliderFlags_Logarithmic);
}

} // namespace Cool