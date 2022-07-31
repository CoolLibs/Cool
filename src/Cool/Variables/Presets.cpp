#include "Presets.h"

// TODO(LD) Add a way to update all presets
// TODO(LD) In case of a rename show a merge window that allows user to explicit the link between old and new names (for each old name that doesn't have a match in the new names, show a dropsown that allows to link it to one of the new names that don't correspond to any old names)

namespace Cool {

auto PresetManager::imgui(PresetData& preset_data) -> bool
{
    std::string current_name = initial_name(_current_preset_id);
    if (ImGui::BeginCombo(
            "Presets",
            current_name.c_str(),
            0
        ))
    {
        for (auto& [id, preset] : _presets)
        {
            bool is_selected = false;
            if (_current_preset_id != std::nullopt)
            {
                is_selected = (*_current_preset_id == id);
            }
            if (ImGui::Selectable(preset.name.c_str(), is_selected))
            {
                _current_preset_id = id;
                if (_presets.get(*_current_preset_id) != std::nullopt)
                {
                    preset_data = _presets.get(*_current_preset_id)->values;
                }
            }
            if (is_selected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
    if (ImGui::Button("Danger"))
    {
        const auto sel = boxer::show("Make a choice:", "Decision", boxer::Style::Warning, boxer::Buttons::OK);
        (void)sel;
    }

    // PresetId deleted_id{};
    // for (auto& [id, preset] : _presets)
    // {
    //     ImGui::PushID(&id);
    //     for (auto& variable : preset.values)
    //     {
    //         std::visit([](auto&& real_variable) { imgui_widget(real_variable); }, variable);
    //     }
    //     ImGui::PopID();
    // }
    // remove_preset(deleted_id); // That works even if we don't pass in the `if` above because remove_preset already verifies if deleted_id is not null

    ImGui::Separator();

    static char name[64] = ""; // TODO(LD) Use a std::string
    ImGui::InputText("Name", name, 64);

    for (auto& variable : preset_data)
    {
        std::visit([](auto&& real_variable) { imgui_widget(real_variable); }, variable);
    }
    if (_current_preset_id != std::nullopt)
    {
        if (ImGui::Button("-"))
        {
            auto id_to_delete = *_current_preset_id;
            remove_preset(id_to_delete); // Crash (bug with current_name ?)
        }
    }

    if (ImGui::Button("+"))
    {
        Preset2 new_preset = {
            .name   = name,
            .values = preset_data};
        add_preset(new_preset);
    }

    return true;
}

} // namespace Cool