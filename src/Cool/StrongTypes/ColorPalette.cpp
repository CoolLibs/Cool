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
    const auto palette_width = std::max(
        static_cast<size_t>(1u),
        static_cast<size_t>((ImGui::CalcItemWidth() - button_size()) / button_size())
    );
    return (index % palette_width != 0);
}

auto imgui_widget(std::string_view name, Cool::ColorPalette& palette, ImGuiColorEditFlags flags) -> bool
{
    bool value_has_changed = false;

    ImGui::Text("%s", name.data());
    ImGui::BeginGroup();
    for (size_t i = 0; i < palette.value.size(); i++)
    {
        ImGui::PushID(static_cast<int>(i));
        if (must_be_in_same_line(i))
        {
            ImGui::SameLine();
        }
        value_has_changed |= imgui_widget("", palette.value[i], ImGuiColorEditFlags_NoInputs | flags);
        if (ImGui::IsMouseReleased(ImGuiPopupFlags_MouseButtonMiddle) && ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup))
        {
            palette.remove_color(i);
        }

        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None | ImGuiDragDropFlags_SourceAllowNullID))
        {
            ImGui::SetDragDropPayload(" ", &i, sizeof(size_t));

            ImGui::EndDragDropSource();
        }
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload_old = ImGui::AcceptDragDropPayload(" "))
            {
                IM_ASSERT(payload_old->DataSize == sizeof(size_t));
                const size_t old_index = *reinterpret_cast<const size_t*>(payload_old->Data);
                palette.move_color(old_index, i);
            }
            ImGui::EndDragDropTarget();
        }

        ImGui::PopID();
    }
    ImGui::EndGroup();
    ImGui::SameLine();
    if (ImGui::Button("+", ImVec2(button_size(), button_size())))
    {
        palette.add_color();
        ImGui::OpenPopup("picker");
        value_has_changed = true;
    }
    if (ImGui::BeginPopup("picker"))
    {
        ImGui::SetNextItemWidth(button_size() * 12.0f);
        auto srgb = palette.value[palette.value.size() - 1].as_srgb();
        value_has_changed |= ImGui::ColorPicker3("##picker", glm::value_ptr(srgb), flags);
        palette.value[palette.value.size() - 1] = Color::from_srgb(srgb);
        ImGui::EndPopup();
    }
    Cool::ImGuiExtras::tooltip("Add a color");
    Cool::ImGuiExtras::tooltip("Remove a color by middle click on it");

    return value_has_changed;
}
} // namespace Cool