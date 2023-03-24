#include "ColorThemes.h"

namespace Cool {

void ColorThemes::imgui()
{
    _editor.widget_theme_picker();
}

void ColorThemes::imgui_advanced_config()
{
    _editor.widget_color_config();
}

} // namespace Cool
