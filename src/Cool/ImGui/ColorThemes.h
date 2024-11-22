#pragma once
#include <ImStyleEd/ImStyleEd.hpp>

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
    ImStyleEd::Editor _editor;
};

} // namespace Cool