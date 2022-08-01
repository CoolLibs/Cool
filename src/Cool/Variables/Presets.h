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

    auto initial_name(const std::optional<PresetId> id) -> std::string
    {
        if (id != std::nullopt)
        {
            if (_presets.get(*id) != std::nullopt)
            {
                return _presets.get(*id)->name;
            }
        }
        return "Unsaved";
    }

    auto imgui(PresetData& preset_data) -> bool;

private:
    reg::OrderedRegistry<Preset2> _presets;
    std::optional<PresetId>       _current_preset_id;
};

} // namespace Cool