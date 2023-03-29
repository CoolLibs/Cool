#include "Gradient.h"

namespace Cool {

auto to_string(Cool::Gradient const&) -> std::string
{
    return "Gradient [...]";
}

auto imgui_widget(std::string_view name, Cool::Gradient& gradient, bool should_use_a_random_color_for_the_new_marks, ImGuiColorEditFlags flags) -> bool
{
    auto modified = gradient.value.widget(
        name.data(),
        {
            .flags                                       = ImGG::Flag::NoResetButton,
            .color_edit_flags                            = flags,
            .should_use_a_random_color_for_the_new_marks = should_use_a_random_color_for_the_new_marks,
            .plus_button_text                            = "+",
            .minus_button_text                           = "-",
        }
    );
    modified |= ImGG::interpolation_mode_widget("Interpolation Mode", &gradient.value.gradient().interpolation_mode());
    ImGui::SameLine();
    modified |= ImGG::wrap_mode_widget("Wrap Mode", &gradient.wrap_mode);
    ImGui::SameLine();
    if (ImGui::Button("Spread marks evenly"))
    {
        gradient.value.gradient().spread_marks_evenly();
        modified = true;
    }
    return modified;
}

namespace internal {
auto gradient_marks_array_name(std::string_view name) -> std::string
{
    return fmt::format("{}_", name);
}
} // namespace internal

} // namespace Cool