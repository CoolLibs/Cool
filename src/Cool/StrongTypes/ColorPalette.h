#pragma once

#include <op/op.hpp>
#include <vector>
#include "RgbColor.h"

namespace Cool {
struct ColorPalette : public op::EqualityComparable<ColorPalette> {
    std::vector<Cool::RgbColor> value{};
    std::vector<glm::vec4>      vec{};
    constexpr ColorPalette() = default;
    explicit ColorPalette(const std::vector<Cool::RgbColor>& value)
        : value{value}
    {}
    void remove_color() { value.pop_back(); }
    void add_color(Cool::RgbColor color) { value.push_back(color); }

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("Palette", value));
    }
};

inline auto palette_widget(const std::string& name, Cool::ColorPalette& palette, ImGuiColorEditFlags flags) -> bool
{
    size_t number_of_colors  = palette.value.size();
    bool   value_has_changed = false;
    for (size_t i = 0; i < number_of_colors - 1; i++)
    {
        ImGui::PushID(i);
        value_has_changed = value_has_changed || color_widget(name, palette.value[i].value, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | flags);
        ImGui::SameLine();
        ImGui::PopID();
    }
    return value_has_changed || color_widget(name, palette.value[number_of_colors - 1].value, ImGuiColorEditFlags_NoInputs | flags);
}

} // namespace Cool
