#pragma once

#include <algorithms/algorithms.hpp>
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

    void remove_color(size_t index)
    {
        assert(index < value.size());
        value.erase(value.begin() + index);
    }

    void add_color(Cool::RgbColor color = {}) { value.push_back(color); }

    void move_color(size_t old_index, size_t new_index)
    {
        algorithms::translocate(std::next(value.begin(), old_index), std::next(value.begin(), new_index));
    }

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("Color Palette", value));
    }
};

inline auto palette_widget(const std::string& name, Cool::ColorPalette& palette, ImGuiColorEditFlags flags) -> bool
{
    const float  button_size       = ImGui::GetFrameHeight();
    size_t       number_of_colors  = palette.value.size();
    bool         value_has_changed = false;
    const float  width             = ImGui::CalcItemWidth();
    const size_t palette_width     = size_t((width - button_size) / button_size);

    ImGui::Text("%s", name.c_str());
    ImGui::BeginGroup();
    for (size_t i = 0; i < number_of_colors; i++)
    {
        ImGui::PushID(int(i));
        if ((i != 0) && (std::fmod(i, palette_width) != 0))
        {
            ImGui::SameLine();
        }
        value_has_changed |= color_widget("", palette.value[i].value, ImGuiColorEditFlags_NoInputs | flags);
        if (ImGui::IsMouseReleased(ImGuiPopupFlags_MouseButtonMiddle) && ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup))
        {
            palette.remove_color(i);
            number_of_colors--;
        }

        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None | ImGuiDragDropFlags_SourceAllowNullID))
        {
            ImGui::SetDragDropPayload(" ", &i, sizeof(int));

            ImGui::EndDragDropSource();
        }
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(" "))
            {
                IM_ASSERT(payload->DataSize == sizeof(int));
                int payload_n = *(const int*)payload->Data;
                palette.move_color(payload_n, i);
            }
            ImGui::EndDragDropTarget();
        }

        ImGui::PopID();
    }
    ImGui::EndGroup();
    ImGui::SameLine();
    if (ImGui::Button("+", ImVec2(button_size, button_size)))
    {
        palette.add_color();
        ImGui::OpenPopup("picker");
        value_has_changed = true;
    }
    if (ImGui::BeginPopup("picker"))
    {
        ImGui::SetNextItemWidth(button_size * 12.0f);
        value_has_changed |= ImGui::ColorPicker3("##picker", glm::value_ptr(palette.value[number_of_colors - 1].value), flags);
        ImGui::EndPopup();
    }
    Cool::ImGuiExtras::tooltip("Add a color");
    Cool::ImGuiExtras::tooltip("Remove a color by middle click on it");

    return value_has_changed;
}

} // namespace Cool
