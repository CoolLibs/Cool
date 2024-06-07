#include "MeshExportFormat.hpp"

namespace Cool {

auto enum_name(MeshExportFormat export_format) -> std::string
{
    switch (export_format)
    {
    case MeshExportFormat::ply:
        return ".ply";
    }
    assert(false);
    return "Unknown";
}

// TODO: move in Cool::ImGuiExtras and generalize for enums (using magic_enum)
auto imgui_combo(const char* label, MeshExportFormat& export_format) -> bool
{
    bool              b     = false;
    static const auto items = std::array<const char*, 1>{".ply"};
    auto              current_item{static_cast<int>(export_format)};
    if (ImGui::BeginCombo(label, items[current_item]))
    {
        for (int i = 0; i < static_cast<int>(items.size()); i++)
        {
            bool const is_selected{current_item == i};
            if (ImGui::Selectable(items[i], is_selected))
            {
                current_item = i;
                b            = true;
            }
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    export_format = static_cast<MeshExportFormat>(current_item);
    return b;
}

} // namespace Cool