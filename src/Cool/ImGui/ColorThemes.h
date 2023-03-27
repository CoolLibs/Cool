#pragma once
#include <ImStyleEd/ImStyleEd.hpp>
#include "Cool/Path/Path.h"

namespace Cool {

class ColorThemes {
public:
    void imgui_theme_picker();
    void imgui_basic_theme_editor();
    void imgui_advanced_config();

    void update();

private:
    class OsThemeChecker {
    public:
        void update(ImStyleEd::Editor&);

    private:
        enum class Mode {
            Unknown,
            Dark,
            Light,
        };

    private:
        Mode _color_mode{Mode::Unknown};
    };

private:
    ImStyleEd::Editor _editor{{
        .themes_path = Cool::Path::root() / "color_themes.json",
        .config_path = Cool::Path::root() / "color_config.json",
    }};

    std::optional<OsThemeChecker> _use_os_theme{OsThemeChecker{}};

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    auto save_minimal(Archive const&) const -> bool
    {
        return _use_os_theme.has_value();
    }

    template<class Archive>
    void load_minimal(Archive const&, bool const& value)
    {
        if (value)
            _use_os_theme = OsThemeChecker{};
        else
            _use_os_theme = std::nullopt;
    }
};

} // namespace Cool