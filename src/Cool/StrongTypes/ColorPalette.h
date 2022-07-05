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
    const size_t text_length       = ImGui::CalcTextSize(name.c_str()).y;
    const size_t palette_width     = size_t(width / button_size) - text_length;

    ImGui::Text("%s", name.c_str());
    ImGui::SameLine();
    ImGui::BeginGroup();
    for (size_t i = 0; i < number_of_colors; i++)
    {
        ImGui::PushID(int(i));
        if ((i != 0) && (std::fmod(i, palette_width) != 0))
        {
            ImGui::SameLine();
        }
        value_has_changed |= color_widget("", palette.value[i].value, ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoInputs | flags);
        ImGui::PopID();
    }
    ImGui::EndGroup();

    ImGui::SameLine();
    if (ImGui::Button("+", ImVec2(button_size, button_size)))
    {
        palette.add_color();
        value_has_changed = true;
    }
    Cool::ImGuiExtras::tooltip("Add a color");

    return value_has_changed;
}

} // namespace Cool
