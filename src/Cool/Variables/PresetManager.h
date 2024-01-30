#pragma once

#include <Cool/Serialization/SerializerOnDemand.h>
#include <reg/cereal.hpp>
#include "Settings.h"
#include "Settings_Ref.h"

namespace Cool {

// TODO(JF) Remove the old presets system

struct Preset2 {
    std::string name;
    Settings    values;

    friend auto operator==(const Preset2&, const Preset2&) -> bool = default;

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

/// You might want to separate the presets you ship with your application
/// and the ones defined by your users. So that for example when a user
/// downloads a new version of the app, they will get the new presets you added
/// while easily keeping theirs: no merge required on the presets file since
/// your app and your users use two different files!
///
/// Note that the default presets are read only! Users cannot change them through the UI.
struct PresetsPaths {
    std::filesystem::path user_defined_presets{};
    std::filesystem::path default_presets{};
};

class PresetManager {
public:
    explicit PresetManager(PresetsPaths const&);

    [[nodiscard]] auto path() const -> auto const& { return _serializer.path(); }

    /// Renders the UI for the whole `PresetManager`.
    // auto imgui(Settings_Ref settings) -> bool;

    /// Renders only the UI for the presets.
    auto imgui_presets(Settings_Ref settings) -> bool;

    /// Adds `preset` to the list of presets.
    /// Returns the ID that will allow you to reference that `preset`.
    auto add(Preset2 preset, bool show_warning_messages = true) -> PresetId;

    /// Removes the preset referenced by `id`.
    void remove(const PresetId& id);

    /// Changes the `values` of the preset referenced by `id`.
    void edit(const PresetId& id, const Settings& new_values);

    /// Changes the `name` of the preset referenced by `id`.
    void rename(const PresetId& id, std::string_view new_name);

    /// Applies a preset to some `settings`.
    /// Returns true iff the `id` was valid and the preset has actually been applied.
    auto apply(const PresetId& id, Settings_Ref settings) const -> bool;

    /// Applies the first preset if it exists.
    /// Does nothing if the preset manager is empty.
    void apply_first_preset_if_there_is_one(Settings_Ref settings) const;

    /// Returns true iff `id` references an existing preset.
    auto contains(const PresetId& id) const -> bool { return _default_presets.contains(id) || _user_defined_presets.contains(id); }

    /// Returns the ID of the preset with the given `values`, or a null ID if no preset has the given `values`.
    auto find_preset_with_given_values(Settings_ConstRef values) const -> PresetId;

    /// Returns the ID of the preset with the given `name`, or a null ID if no preset has the given `name`.
    auto find_preset_with_given_name(std::string_view name) const -> PresetId;

    /// Returns the name of the preset referenced by `id`, or nullopt if it doesn't exist.
    auto preset_name(PresetId const& id) const -> std::optional<std::string>;

    auto is_empty() const -> bool { return _default_presets.is_empty() && _user_defined_presets.is_empty(); }

private:
    /// Creates a dropdown containing all the presets.
    /// Returns the ID of the selected preset if any, or a null ID otherwise.
    auto dropdown(
        std::string_view dropdown_name,
        std::string_view current_preset_name,
        ImGuiComboFlags = ImGuiComboFlags_None,
        ImDrawFlags     = 0
    ) -> PresetId;

    /// Arrow which opens a dropdown that will fill _new_preset_name with the name of one of the presets.
    void name_selector();

    /// Displays UI to add a preset in PresetManager with add_button, an input text and name_selector.
    void imgui_adding_preset(Settings_ConstRef);

    /// Creates an InputText to write the _new_preset_name and an arrow to fill it with an already existing preset's name.
    /// Returns true if we should create a preset.
    auto imgui_name_input() -> bool;

    /// Creates a new preset, or tries to overwrite the preset referenced by `id` if the later is valid.
    void save_preset(Settings_ConstRef new_preset_values, const PresetId& id);

    /// Writes the current user-defined presets to the file.
    void save_to_file();

    /// Returns the ID of the first Preset that matches the `predicate`, or a null ID if there was none.
    auto find_preset(std::function<bool(const Preset2&)> const& predicate) const -> PresetId;

private:
    class RenamerWidget {
    public:
        void imgui(const PresetId& id, std::string_view current_name, PresetManager& preset_manager);

    private:
        std::string _new_name;
        bool        _popup_is_open{false};
    };

private:
    reg::RawOrderedRegistry<Preset2> _default_presets;
    reg::RawOrderedRegistry<Preset2> _user_defined_presets;
    PresetId                         _current_preset_id;
    std::string                      _new_preset_name;
    RenamerWidget                    _rename_widget;
    Cool::SerializerOnDemand         _serializer;

private:
    // Serialization
    // Actually this class is not serialized automatically, but serializes its two reg::RawOrderedRegistry<Preset2> manually.
};

} // namespace Cool