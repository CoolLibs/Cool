#pragma once

namespace Cool::ImGuiExtras {

template<typename DropdownEntry>
auto dropdown(const char* label, const char* preview, std::vector<DropdownEntry>& entries) -> bool
{
    bool b{false};

    if (ImGui::BeginCombo(label, preview))
    {
        for (auto& entry : entries)
        {
            bool const selected = entry.is_selected();
            if (ImGui::Selectable(entry.get_label(), selected))
            {
                entry.apply_value();
                b = true;
            }
            if (selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    if (ImGui::IsItemHovered() && ImGui::GetIO().MouseWheel != 0.f)
    {
        bool const find_bigger = ImGui::GetIO().MouseWheel > 0.f;
        int const  sz          = static_cast<int>(entries.size());
        bool       select_next{false};
        for (int i = find_bigger ? sz - 1 : 0; find_bigger ? (i >= 0) : (i < sz); i += find_bigger ? -1 : 1)
        {
            if (select_next)
            {
                entries[i].apply_value();
                b = true;
                break;
            }
            select_next = entries[i].is_selected();
        }
    }

    return b;
}

} // namespace Cool::ImGuiExtras