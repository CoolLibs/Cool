#pragma once

#include <Cool/Variables/AnyVariable.h>

namespace Cool {

using PresetData = std::vector<Cool::AnyVariable>;

struct Preset {
    std::string name;
    PresetData  values;
};

using PresetId = reg::Id<Preset>;

class PresetManager {
public:
    auto add_preset(const Preset& preset) -> PresetId
    {
        return _presets.create(preset);
    }

    auto remove_preset(const PresetId& id)
    {
        _presets.destroy(id);
    }

    // TODO(LD) Add a way to update all presets
    // TODO(LD) In case of a rename show a merge window that allows user to explicit the link between old and new names (for each old name that doesn't have a match in the new names, show a dropsown that allows to link it to one of the new names that don't correspond to any old names)

    auto imgui(PresetData& preset_data) -> bool
    {
        // TODO(LD) Dropdown to select a preset
        PresetId deleted_id{};
        for (const auto& [id, preset] : _presets)
        {
            if (ImGui::Button(preset.name.c_str()))
            {
                deleted_id = id;
            }
        }
        remove_preset(deleted_id); // That works even if we don't pass in the `if` above because remove_preset already verifies if deleted_id is not null

        // TODO(LD) Button to add a preset.
        for (auto& variable : preset_data)
        {
            std::visit([](auto&& real_variable) { imgui_widget(real_variable); }, variable);
        }

        if (ImGui::Button("+"))
        {
            // add_preset
        }
        return true;
    }

private:
    reg::OrderedRegistry<Preset> _presets;
    std::optional<size_t>        _current_preset_index;
};

} // namespace Cool