#include "ColorThemes.h"
#include <imgui.h>
#include <wants_dark_theme/wants_dark_theme.hpp>
#include "Cool/ImGui/ImGuiExtras.h"

namespace Cool {

void ColorThemes::imgui_theme_picker()
{
    {
        bool is_using_os_theme = _use_os_theme.has_value();
        if (ImGuiExtras::toggle("Use OS color theme", &is_using_os_theme))
        {
            if (is_using_os_theme)
                _use_os_theme.emplace();
            else
                _use_os_theme.reset();
        }
    }

    ImGuiExtras::maybe_disabled(
        _use_os_theme.has_value(),
        "You are using the OS color theme. Disable the option above to edit your theme freely.",
        [&]() {
            _editor.widget_theme_picker();
        }
    );
}

void ColorThemes::imgui_basic_theme_editor()
{
    _editor.widget_theme_picker();
    _editor.widget_theme_editor();
}

void ColorThemes::imgui_advanced_config()
{
    _editor.widget_color_config();
}

void ColorThemes::update()
{
    if (!_use_os_theme)
        return;
    _use_os_theme->update(_editor);
}

void ColorThemes::OsThemeChecker::update(ImStyleEd::Editor& editor)
{
    auto const prev_color_mode = _color_mode;
    _color_mode                = wants_dark_theme().value_or(true) ? Mode::Dark : Mode::Light;
    if (_color_mode == prev_color_mode)
        return;

    if (_color_mode == Mode::Light)
        editor.apply_if_any("Light");
    else
        editor.apply_if_any("Dark");
}

} // namespace Cool
