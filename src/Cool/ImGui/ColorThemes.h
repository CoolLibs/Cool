#pragma once
#include "Cool/Serialization/JsonAutoSerializer.hpp"
#include "ImStyleEd/ImStyleEd.hpp"

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

    // JsonAutoSerializer _serializer; // Must be declared last, after all the variables it serializes, so that the values it loads overwrite the default values, and not the other way around
};

inline auto color_themes() -> std::optional<ColorThemes>& // It is optional because we want to control its lifetime (it must be destroyed before destroying the ImGui context to make sure it can still access the ImGuiStyle to serialize it)
{
    static auto instance = std::optional<ColorThemes>{};
    return instance;
}

} // namespace Cool