#pragma once
#include <ImStyleEd/ImStyleEd.hpp>
#include "Cool/Path/Path.h"

namespace Cool {

class ColorThemes {
public:
    ColorThemes();

    void imgui_theme_picker();
    void imgui_basic_theme_editor();
    void imgui_advanced_config();

    void update();

    auto editor() const -> auto const& { return _editor; }

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
    ImStyleEd::Editor _editor;

    std::optional<OsThemeChecker> _use_os_theme{OsThemeChecker{}};

private:
    // Serialization
    friend class ser20::access;
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