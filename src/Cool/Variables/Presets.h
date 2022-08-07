#pragma once

#include <Cool/Serialization/AutoSerializer.h>
#include <Cool/Serialization/as_json.h>
#include <Cool/Variables/AnyVariable.h>
#include <boxer/boxer.h>
#include <cereal/types/optional.hpp>
#include <cereal/types/variant.hpp>
#include <reg/cereal.hpp>

namespace Cool {

// TODO(LD) Documentation
using Settings = std::vector<Cool::AnyVariable>;

struct Preset2 {
    std::string name;
    Settings    values;

    friend bool operator==(const Preset2&, const Preset2&) = default;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Name", name),
            cereal::make_nvp("Values", values)
        );
    }
};

using PresetId = reg::Id<Preset2>;

// TODO(LD) Documentation
class PresetManager {
public:
    PresetManager(std::filesystem::path path)
        : _auto_serializer{path, "PresetManager", *this}
    {}

    auto get_id_by_given_values(Settings& values) -> PresetId
    {
        for (auto& [id, preset] : _presets)
        {
            if (preset.values == values)
            {
                return id;
            }
        }
        return PresetId{};
    }

    auto contains(const PresetId& id) -> bool
    {
        return _presets.contains(id);
    }

    // TODO(LD) Documentation
    auto add_preset(const Preset2& preset) -> PresetId
    {
        return _presets.create(preset);
    }

    // TODO(LD) Documentation
    void remove_preset(const PresetId& id)
    {
        _presets.destroy(id);
    }

    // TODO(LD) Documentation
    void edit_preset_values(const PresetId& id, const Settings& settings);

    // TODO(LD) Documentation
    void rename_preset(const PresetId& id, std::string_view name);

    // TODO(LD) Documentation
    auto imgui(Settings& preset_data) -> bool;

private:
    auto current_name(
        const PresetId& id,
        const Settings& current_settings
    ) -> std::string;

    auto dropdown(
        std::string_view dropdown_name,
        std::string_view current_name,
        Settings*        preset_data,
        ImGuiComboFlags_ flag
    ) -> bool;

    auto name_selector() -> bool;

    auto add_button(Settings& preset_data) -> bool;

    void apply_preset(PresetId id, Settings& preset_data);

private:
    reg::OrderedRegistry<Preset2> _presets;
    PresetId                      _current_preset_id;
    std::string                   _new_preset_name;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Presets", _presets)
        );
    }

private:
    Cool::AutoSerializer<PresetManager> _auto_serializer; // Must be last
};

} // namespace Cool