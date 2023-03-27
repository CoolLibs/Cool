#include "ColorThemes.h"
#include <imgui.h>
#include <wants_dark_theme/wants_dark_theme.hpp>

namespace Cool {

void ColorThemes::imgui()
{
    _editor.widget_theme_picker();
    if (ImGui::Button("Apply OS Preferences"))
        apply_os_preferences();
}

void ColorThemes::imgui_advanced_config()
{
    _editor.widget_color_config();
}

void ColorThemes::apply_os_preferences()
{
    if (wants_dark_theme().value_or(true))
        _editor.apply_if_any("Dark");
    else
        _editor.apply_if_any("Light");
}

} // namespace Cool
