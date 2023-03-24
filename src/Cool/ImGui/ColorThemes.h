#pragma once
#include <ImStyleEd/ImStyleEd.hpp>
#include "Cool/Path/Path.h"

namespace Cool {

class ColorThemes {
public:
    void imgui();
    void imgui_advanced_config();

private:
    ImStyleEd::Editor _editor{{
        .themes_path = Cool::Path::root() / "color_themes.json",
        .config_path = Cool::Path::root() / "color_config.json",
    }};
};

} // namespace Cool