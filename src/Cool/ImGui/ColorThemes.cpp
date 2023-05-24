#include "ColorThemes.h"
#include <imgui-node-editor/imgui_node_editor.h>
#include <imgui.h>
#include <wants_dark_theme/wants_dark_theme.hpp>
#include "Cool/ImGui/ImGuiExtras.h"
#include "ImGuiExtrasStyle.h"

namespace Cool {

static void register_imgui_extras_elements(ImStyleEd::Config& config);

ColorThemes::ColorThemes()
    : _editor{
        ImStyleEd::SerializationPaths{
            .current_theme = Cool::Path::root() / "current_color_theme.json",
            .themes        = Cool::Path::root() / "color_themes.json",
            .config        = Cool::Path::root() / "color_config.json",
        },
        [](ImStyleEd::Config& config) {
            ImStyleEd::register_all_imgui_color_elements(config);
            register_imgui_extras_elements(config);
        }}
{
}

void ColorThemes::imgui_theme_picker()
{
    {
        bool is_using_os_theme = _use_os_theme.has_value();
        if (ImGuiExtras::toggle("Use OS color theme", &is_using_os_theme))
        {
            if (is_using_os_theme)
                _use_os_theme = OsThemeChecker{};
            else
                _use_os_theme = std::nullopt;
        }
    }

    ImGuiExtras::disabled_if(
        _use_os_theme.has_value(),
        "You are using the OS color theme. Disable the option above to edit your theme freely.",
        [&]() {
            _editor.imgui_theme_selector();
        }
    );
}

void ColorThemes::imgui_basic_theme_editor()
{
    _editor.imgui_themes_editor();
}

void ColorThemes::imgui_advanced_config()
{
    _editor.imgui_config_editor();
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
        editor.apply_theme_if_any("Light");
    else
        editor.apply_theme_if_any("Dark");
}

static void register_imgui_extras_elements(ImStyleEd::Config& config)
{
#include "generated_style/register_elements.inl"
#include "generated_style_nodes/register_all_imnodes_color_elements.inl"
}

} // namespace Cool
