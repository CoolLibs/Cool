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

    /// Searches if PresetManager contains a preset which have same values as given `values`.
    auto find_preset_with_given_values(const Settings& values) const -> PresetId;

    /// Return if PresetManager contains preset identified by the given `id`.
    auto contains(const PresetId& id) const -> bool { return _presets.contains(id); }

    /// Add given preset in the PresetManager.
    auto add_preset(const Preset2& preset) -> PresetId { return _presets.create(preset); }

    /// Remove preset identified by the given id from the PresetManager.
    void remove_preset(const PresetId& id) { _presets.destroy(id); }

    /// Change settings values by given `settings` from preset identified by the given `id`.
    void edit_preset_values(const PresetId& id, const Settings& settings);

    /// Change name by given `name` from preset identified by the given `id`.
    void rename_preset(const PresetId& id, std::string_view name);

    /// Main loop that creates presets interface with current settings, list of presets, add and delete button.
    auto imgui(Settings& settings) -> bool;

private:
    /// Name of the preset identified by given id.
    auto preset_name(const PresetId&) const -> std::string;

    /// Creates a dropdown containing all the presets of PresetManager.
    auto dropdown(
        std::string_view dropdown_name,
        std::string_view current_name,
        ImGuiComboFlags = ImGuiComboFlags_None
    ) -> PresetId;

    /// Arrow which calls a dropdown to fill _new_preset_name by one of the preset's name already existing.
    void name_selector();

    /// Displays interface to add a preset in PresetManager with add_button, an input text and name_selector.
    void adding_preset_interface(const Settings&);

    /// Apply preset identified by the given id on current settings.
    auto apply_preset(const PresetId&, Settings&) const -> bool;

    /// Searches if a preset contained by PresetManager have a same name as `name`.
    auto find_preset_with_same_name(std::string_view name) -> Preset2*;

    /// Creates an InputText to write the _new_preset_name and an arrow to fill it with an already existing preset's name.
    void input_name();

    /// Print an alert window to ask you if you really want to replace settings of the given `preset` by the given `settings` and do it if you click yes.
    void overwrite_preset(Preset2* preset, const Settings& settings);

    /// Add `preset` to PresetManager if `preset` is nullptr, calls overwrite_preset otherwise.
    void save_preset(Preset2* preset, const Settings& settings);

private:
    class RenameWidget {
    public:
        /// Button which permits to edit the current preset's name.
        void rename_button(PresetId& id, std::string_view current_name, PresetManager& preset_manager);

    private:
        std::string _new_name;
        bool        _is_popup_open = false;
    };

private:
    reg::OrderedRegistry<Preset2> _presets;
    PresetId                      _current_preset_id;
    std::string                   _new_preset_name;
    RenameWidget                  _rename_widget;

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
    Cool::AutoSerializer<PresetManager> _auto_serializer; // Must be last to properly serialize the class
};

} // namespace Cool