#pragma once

#include <Cool/Serialization/AutoSerializer.h>
#include <Cool/Variables/AnyVariable.h>
#include <reg/cereal.hpp>

namespace Cool {

// TODO(JF) TODO(LD) Remove the old presets system

/// List of values contained by a preset.
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

class PresetManager {
public:
    PresetManager(std::filesystem::path path)
        : _auto_serializer{path, "PresetManager", *this, [](const std::string&) {
                               /*Ignore deserialization warnings*/
                           }}
    {}

    auto path() const { return _auto_serializer.path(); }

    /// Renders the UI for the whole `PresetManager`.
    auto imgui(Settings& settings) -> bool;

    /// Renders only the UI for the presets.
    auto imgui_presets(Settings& settings) -> bool;

    /// Adds `preset` to the list of presets.
    /// Returns the ID that will allow you to reference that `preset`.
    auto add(const Preset2& preset, bool show_warning_messages = true) -> PresetId;

    /// Removes the preset referenced by `id`.
    void remove(const PresetId& id) { _presets.destroy(id); }

    /// Changes the `values` of the preset referenced by `id`.
    void edit(const PresetId& id, const Settings& new_values);

    /// Changes the `name` of the preset referenced by `id`.
    void rename(const PresetId& id, std::string_view new_name);

    /// Applies a preset to some `settings`.
    /// Returns true iff the `id` was valid and the preset has actually been applied.
    auto apply(const PresetId& id, Settings& settings) const -> bool;

    /// Applies the first preset if it exists.
    /// Does nothing if the preset manager is empty.
    void apply_first_preset_if_there_is_one(Settings& settings) const;

    /// Returns true iff `id` references an existing preset.
    auto contains(const PresetId& id) const -> bool { return _presets.contains(id); }

    /// Returns the ID of the preset with the given `values`, or a null ID if no preset has the given `values`.
    auto find_preset_with_given_values(const Settings& values) const -> PresetId;

    /// Returns the ID of the preset with the given `name`, or a null ID if no preset has the given `name`.
    auto find_preset_with_given_name(std::string_view name) const -> PresetId;

    /// Returns the name of the preset referenced by `id`, or nullopt if it doesn't exist.
    auto preset_name(const PresetId& id) const -> std::optional<std::string>;

private:
    /// Creates a dropdown containing all the presets.
    /// Returns the ID of the selected preset if any, or a null ID otherwise.
    auto dropdown(
        std::string_view dropdown_name,
        std::string_view current_preset_name,
        ImGuiComboFlags = ImGuiComboFlags_None
    ) -> PresetId;

    /// Arrow which opens a dropdown that will fill _new_preset_name with the name of one of the presets.
    void name_selector();

    /// Displays UI to add a preset in PresetManager with add_button, an input text and name_selector.
    void imgui_adding_preset(const Settings&);

    /// Creates an InputText to write the _new_preset_name and an arrow to fill it with an already existing preset's name.
    void imgui_name_input();

    /// Creates a new preset, or tries to overwrite the preset referenced by `id` if the later is valid.
    void save_preset(const Settings& new_preset_values, const PresetId& id);

private:
    class RenamerWidget {
    public:
        void imgui(const PresetId& id, std::string_view current_name, PresetManager& preset_manager);

    private:
        std::string _new_name;
        bool        _popup_is_open{false};
    };

private:
    reg::OrderedRegistry<Preset2> _presets;
    PresetId                      _current_preset_id;
    std::string                   _new_preset_name;
    RenamerWidget                 _rename_widget;

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