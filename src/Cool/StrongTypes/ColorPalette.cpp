#include "ColorPalette.h"
#include <Cool/ImGui/ImGuiExtras.h>
#include <imgui/imgui_internal.h>
namespace Cool {

static auto button_size() -> float
{
    return ImGui::GetFrameHeight();
}

auto must_be_in_same_line(size_t index) -> bool
{
    const auto palette_width = std::max(static_cast<size_t>(1u), static_cast<size_t>((ImGui::CalcItemWidth() - button_size()) / button_size()));
    return (index % palette_width != 0);
}

auto imgui_color_palette_widget(std::string_view name, Cool::ColorPalette& palette, ImGuiColorEditFlags flags) -> bool
{
    const float  button_size       = ImGui::GetFrameHeight();
    size_t       number_of_colors  = palette.value.size();
    bool         value_has_changed = false;
    const float  width             = ImGui::CalcItemWidth();
    const size_t palette_width     = size_t((width - button_size) / button_size);

    ImGui::Text("%s", name.data());
    ImGui::BeginGroup();
    for (size_t i = 0; i < number_of_colors; i++)
    {
        ImGui::PushID(int(i));
        if ((i != 0) && (std::fmod(i, palette_width) != 0))
        {
            ImGui::SameLine();
        }
        value_has_changed |= ImGui::ColorEdit3("", glm::value_ptr(palette.value[i].value), ImGuiColorEditFlags_NoInputs | flags);
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
            if (const ImGuiPayload* payload_old = ImGui::AcceptDragDropPayload(" "))
            {
                IM_ASSERT(payload_old->DataSize == sizeof(int));
                int old_index = *(const int*)payload_old->Data;
                palette.move_color(old_index, i);
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