#include "ColorThemes.h"
#include <imgui.h>
#include <wants_dark_theme/wants_dark_theme.hpp>
#include "Cool/ImGui/ImGuiExtras.h"
#include "ImGuiExtrasStyle.h"

namespace Cool {

static void register_imgui_extras_elements(ImStyleEd::Config& config);

ColorThemes::ColorThemes()
    : _editor{
        ImStyleEd::SerializationPaths{
            .current_theme = Cool::Path::root() / "color_theme_current.json",
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

    ImGuiExtras::maybe_disabled(
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
    config.register_element(ImStyleEd::Element{
        .name        = "Toggle",
        .get_color   = []() { return ImGuiExtras::GetStyle().toggle; },
        .set_color   = [](ImVec4 const& color) { ImGuiExtras::GetStyle().toggle = color; },
        .description = "",
    });
    config.register_element(ImStyleEd::Element{
        .name        = "ToggleHovered",
        .get_color   = []() { return ImGuiExtras::GetStyle().toggle_hovered; },
        .set_color   = [](ImVec4 const& color) { ImGuiExtras::GetStyle().toggle_hovered = color; },
        .description = "",
    });
    config.register_element(ImStyleEd::Element{
        .name        = "ToggleBg",
        .get_color   = []() { return ImGuiExtras::GetStyle().toggle_bg; },
        .set_color   = [](ImVec4 const& color) { ImGuiExtras::GetStyle().toggle_bg = color; },
        .description = "",
    });
    config.register_element(ImStyleEd::Element{
        .name        = "ToggleBgHovered",
        .get_color   = []() { return ImGuiExtras::GetStyle().toggle_bg_hovered; },
        .set_color   = [](ImVec4 const& color) { ImGuiExtras::GetStyle().toggle_bg_hovered = color; },
        .description = "",
    });
    config.register_element(ImStyleEd::Element{
        .name        = "HighlightItems",
        .get_color   = []() { return ImGuiExtras::GetStyle().highlight_items; },
        .set_color   = [](ImVec4 const& color) { ImGuiExtras::GetStyle().highlight_items = color; },
        .description = "Used for example when hovering some error messages to highlight the part of the UI that needs to be used in order to fix the error message.\nUse the \"Test Message Console\" debug option to send such an error message.",
    });
}

} // namespace Cool
