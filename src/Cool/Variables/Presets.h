#pragma once

#include <Cool/Variables/AnyVariable.h>
#include <boxer/boxer.h>

namespace Cool {

using PresetData = std::vector<Cool::AnyVariable>;

struct Preset2 {
    std::string name;
    PresetData  values;

    friend bool operator==(const Preset2&, const Preset2&) = default;
};

using PresetId = reg::Id<Preset2>;

class PresetManager {
public:
    auto add_preset(const Preset2& preset) -> PresetId
    {
        return _presets.create(preset);
    }

    void remove_preset(const PresetId& id)
    {
        _presets.destroy(id);
    }

    auto current_name(
        const std::optional<PresetId> id,
        PresetData&                   preset_data
    ) -> std::string;

    auto imgui(PresetData& preset_data) -> bool;

    void set_preset_data(PresetId id, PresetData& preset_data);

    void dropdown(
        std::string_view           dropdown_name,
        std::string_view           current_name,
        std::optional<PresetData>& preset_data
    );

    void name_selector();

    auto display_all_variables_widgets(PresetData& preset_data);

    auto delete_button(PresetData& preset_data);

    auto add_button(PresetData& preset_data);

private:
    reg::OrderedRegistry<Preset2> _presets;
    std::optional<PresetId>       _current_preset_id;
    std::string                   _name_selector = "none";
    std::string                   _new_preset_name;
};

} // namespace Cool