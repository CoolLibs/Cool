#pragma once

#include <op/op.hpp>
#include <vector>
#include "RgbColor.h"

namespace Cool {
struct ColorPalette : public op::EqualityComparable<ColorPalette> {
    std::vector<Cool::RgbColor> value{};
    constexpr ColorPalette() = default;
    explicit ColorPalette(const std::vector<Cool::RgbColor>& value)
        : value{value}
    {}
    void remove_color() { value.pop_back(); }
    void add_color() { value.push_back(Cool::RgbColor{glm::vec3{1., 1., 1.}}); }
    int  number_of_color() { return value.size() - 1; }

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
    const float  button_size       = ImGui::GetFrameHeight();
    size_t       number_of_colors  = palette.value.size();
    bool         value_has_changed = false;
    const float  width             = ImGui::CalcItemWidth();
    const size_t palette_width     = size_t(width / button_size);

    for (size_t i = 0; i < number_of_colors - 1; i++)
    {
        ImGui::PushID(int(i));
        if ((i != 0) && (i % palette_width != 0))
        {
            ImGui::SameLine();
        }
        value_has_changed = value_has_changed || color_widget(name, palette.value[i].value, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | flags);
        ImGui::PopID();
    }
    if (ImGui::Button("-", ImVec2(button_size, button_size)) && number_of_colors > 1)
    {
        palette.remove_color();
        value_has_changed = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("+", ImVec2(button_size, button_size)))
    {
        palette.add_color();
        value_has_changed = true;
    }

    return value_has_changed || color_widget(name, palette.value[number_of_colors - 1].value, ImGuiColorEditFlags_NoInputs | flags);
}

} // namespace Cool
