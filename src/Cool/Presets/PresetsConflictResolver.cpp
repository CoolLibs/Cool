#include "PresetsConflictResolver.h"

namespace Cool {

auto PresetsConflictResolver::imgui() -> bool
{
    static const char* modal_name = "Presets Conflict";
    if (has_conflicts())
        ImGui::OpenPopup(modal_name);
    if (ImGui::BeginPopupModal(modal_name))
    {
        ImGui::ColorEdit4("colorbg", (float*)&ImGui::GetStyle().Colors[ImGuiCol_ModalWindowDimBg]);
        if (ImGui::Button("Close"))
            ImGui::CloseCurrentPopup();
        ImGui::EndPopup();
    }

    return false;
}

} // namespace Cool