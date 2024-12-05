#pragma once

namespace Cool::ImGuiExtras {

template<typename T>
auto dropdown(const char* label, const char* preview, std::vector<T> const& values, std::function<bool(T const&)> const& is_selected, std::function<const char*(T const&)> const& get_label, std::function<void(T const&)> const& set_value) -> bool
{
    bool b{false};

    if (ImGui::BeginCombo(label, preview))
    {
        for (auto const& value : values)
        {
            bool const selected = is_selected(value);
            if (ImGui::Selectable(get_label(value), selected))
            {
                set_value(value);
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
        int const  sz          = static_cast<int>(values.size());
        bool       select_next{false};
        for (int i = find_bigger ? sz - 1 : 0; find_bigger ? (i >= 0) : (i < sz); i += find_bigger ? -1 : 1)
        {
            if (select_next)
            {
                set_value(values[i]);
                b = true;
                break;
            }
            select_next = is_selected(values[i]);
        }
    }

    return b;
}

} // namespace Cool::ImGuiExtras