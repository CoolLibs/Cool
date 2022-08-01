#include "Presets.h"

// TODO(LD) Add a way to update all presets
// TODO(LD) In case of a rename show a merge window that allows user to explicit the link between old and new names (for each old name that doesn't have a match in the new names, show a dropsown that allows to link it to one of the new names that don't correspond to any old names)

namespace Cool {

auto PresetManager::initial_name(
    const std::optional<PresetId> id,
    PresetData&                   preset_data
) -> std::string
{
    if (id != std::nullopt)
    {
        if (_presets.get(*id) != std::nullopt)
        {
            if (_presets.get(*id)->values != preset_data)
            {
                return "Unsaved";
            }
            else
            {
                return _presets.get(*id)->name;
            }
        }
    }
    return "Unsaved";
}

void PresetManager::dropdown(std::string current_name, PresetData& preset_data)
{
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
                if (_presets.get(*_current_preset_id) != std::nullopt)
                {
                    is_selected &= _presets.get(*_current_preset_id)->name == current_name;
                }
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
}

auto PresetManager::display_all_variables_widgets(PresetData& preset_data)
{
    for (auto& variable : preset_data)
    {
        std::visit([](auto&& real_variable) { imgui_widget(real_variable); }, variable);
    }
}

auto PresetManager::delete_button(PresetData& preset_data)
{
    if (_current_preset_id != std::nullopt)
    {
        if (_presets.get(*_current_preset_id) != std::nullopt)
        {
            if (_presets.get(*_current_preset_id)->values == preset_data)
            {
                if (ImGui::Button("-"))
                {
                    auto id_to_delete = *_current_preset_id;
                    remove_preset(id_to_delete);
                }
            }
        }
    }
}

auto PresetManager::add_button(PresetData& preset_data)
{
    if (ImGui::Button("+"))
    {
        if (_new_preset_name == "")
        {
            boxer::show("You have to name your preset before save.", "No name preset !", boxer::Style::Error, boxer::Buttons::OK);
        }
        else
        {
            Preset2 new_preset = {
                .name   = _new_preset_name,
                .values = preset_data};
            _current_preset_id = add_preset(new_preset);
            _new_preset_name   = "";
        }
    }
}

auto PresetManager::imgui(PresetData& preset_data) -> bool
{
    dropdown(
        initial_name(_current_preset_id, preset_data),
        preset_data
    );
    ImGui::Separator();
    ImGui::InputText("Name", &_new_preset_name);
    display_all_variables_widgets(preset_data);
    delete_button(preset_data);
    add_button(preset_data);

    return true;
}

} // namespace Cool