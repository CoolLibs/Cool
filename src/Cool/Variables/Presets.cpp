#include "Presets.h"
#include <Cool/ImGui/ImGuiExtras.h>
#include <boxer/boxer.h>

// TODO(LD) In case of a change of Settings definition show a merge window that allows user to explicit the link between old and new names (for each old name that doesn't have a match in the new names, show a dropsown that allows to link it to one of the new names that don't correspond to any old names)

// TODO(LD) When renaming to the same name, but show the warning
namespace Cool {

/// Returns the ID of the first Preset that matches the `predicate`, or a null ID if there was none.
static auto find_preset(
    const reg::OrderedRegistry<Preset2>& presets,
    std::function<bool(const Preset2&)>  predicate
) -> PresetId
{
    std::shared_lock lock{presets.mutex()};
    for (const auto& [id, preset] : presets)
    {
        if (predicate(preset))
        {
            return id;
        }
    }
    return PresetId{};
}

auto PresetManager::find_preset_with_given_values(const Settings& values) const -> PresetId
{
    return find_preset(
        _presets, [&](const Preset2& preset) {
            return preset.values == values;
        }
    );
}

auto PresetManager::find_preset_with_given_name(std::string_view name) const -> PresetId
{
    return find_preset(
        _presets, [&](const Preset2& preset) {
            return preset.name == name;
        }
    );
}

// TODO(LD) Add this to our test window!
void PresetManager::edit(const PresetId& id, const Settings& new_values)
{
    if (find_preset_with_given_values(new_values) == PresetId{}) // Make sure there isn't already a preset with the same values.
    {
        _presets.with_mutable_ref(id, [&](Preset2& preset) {
            preset.values = new_values;
        });
    }
    else
    {
        boxer::show(
            "There is already a preset with the same values.",
            "Editing failed",
            boxer::Style::Warning,
            boxer::Buttons::OK
        );
    }
}

void PresetManager::rename(const PresetId& id, std::string_view new_name)
{
    if (find_preset_with_given_name(new_name) == PresetId{}) // Make sure there isn't already a preset with the same name.
    {
        _presets.with_mutable_ref(id, [&](Preset2& preset) {
            preset.name = new_name;
        });
    }
    else
    {
        boxer::show(
            fmt::format("There is already a preset called \"{}\".", new_name).c_str(),
            "Renaming failed",
            boxer::Style::Warning,
            boxer::Buttons::OK
        );
    }
}

auto PresetManager::preset_name(const PresetId& id) const -> std::string
{
    // Default name
    std::string name{"Unsaved Settings..."};

    // Replace default name with preset name if it is found in the registry
    _presets.with_ref(id, [&](const Preset2& preset) {
        name = preset.name;
    });

    return name;
}

auto PresetManager::apply(const PresetId& id, Settings& settings) const -> bool
{
    return _presets.with_ref(id, [&](const Preset2& preset) {
        settings = preset.values;
    });
}

auto PresetManager::dropdown(
    std::string_view dropdown_name,
    std::string_view current_preset_name,
    ImGuiComboFlags  combo_flags
) -> PresetId
{
    auto selected_id = PresetId{};

    if (ImGui::BeginCombo(
            dropdown_name.data(),
            current_preset_name.data(),
            combo_flags
        ))
    {
        for (const auto& [id, preset] : _presets)
        {
            ImGui::PushID(&preset);
            const bool is_selected = _current_preset_id == id;
            if (ImGui::Selectable(preset.name.c_str(), is_selected))
            {
                selected_id = id;
            }
            ImGui::PopID();
        }
        ImGui::EndCombo();
    }
    return selected_id;
}

void PresetManager::name_selector()
{
    const auto id = dropdown(
        "##465",
        "",
        ImGuiComboFlags_NoPreview // Draw just the arrow of the dropdown
    );

    // Update _new_preset_name if a preset was selected in the dropdown
    _presets.with_ref(id, [&](const Preset2& preset) {
        _new_preset_name = preset.name;
    });
}

static auto display_all_variables_widgets(Settings& settings) -> bool
{
    bool was_used{false};
    for (auto& variable : settings)
    {
        was_used |= std::visit([](auto&& real_variable) { return imgui_widget(real_variable); }, variable);
    }
    return was_used;
}

static void delete_button(const PresetId& id, std::string_view name, PresetManager& preset_manager)
{
    if (ImGui::Button("Delete Preset"))
    {
        const auto choice = boxer::show(
            fmt::format("You are about to delete \"{}\", are you sure?", name).c_str(),
            "Are you sure?",
            boxer::Style::Warning,
            boxer::Buttons::YesNo
        );
        if (choice == boxer::Selection::Yes)
        {
            preset_manager.remove(id);
        }
    }
}

void PresetManager::RenamerWidget::imgui(const PresetId& id, std::string_view current_name, PresetManager& preset_manager)
{
    const bool was_open_last_frame{_popup_is_open};
    _popup_is_open = ImGuiExtras::begin_popup_context_menu_from_button("Rename");
    if (_popup_is_open)
    {
        if (!was_open_last_frame)
        {
            _new_name = current_name;
            ImGui::SetKeyboardFocusHere();
        }
        if (ImGui::InputText("##645", &_new_name, ImGuiInputTextFlags_EnterReturnsTrue))
        {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
    if (was_open_last_frame && !_popup_is_open) // Was just closed
    {
        preset_manager.rename(id, _new_name);
    }
}

static auto add_button(bool the_name_is_not_already_used, std::string_view name) -> bool
{
    auto is_clicked{false};
    Cool::ImGuiExtras::maybe_disabled(name == "", "Can't create a preset with an empty name.", [&]() {
        if (the_name_is_not_already_used)
        {
            is_clicked = ImGui::Button("Save Preset");
        }
        else
        {
            is_clicked = Cool::ImGuiExtras::colored_button("Overwrite", 0.5f);
        }
    });
    return is_clicked;
}

void PresetManager::imgui_name_input()
{
    name_selector();
    ImGui::SameLine(0.f, 0.f);
    ImGui::InputText("Name", &_new_preset_name);
}

static auto make_sure_the_user_wants_to_overwrite_the_preset(std::string_view new_preset_name) -> bool
{
    return boxer::show(
               fmt::format("You are about to overwrite \"{}\", are you sure?", new_preset_name).c_str(),
               "Are you sure?",
               boxer::Style::Warning,
               boxer::Buttons::YesNo
           ) == boxer::Selection::Yes;
}

void PresetManager::save_preset(const Settings& new_preset_values, const PresetId& id)
{
    if (contains(id))
    {
        if (make_sure_the_user_wants_to_overwrite_the_preset(_new_preset_name))
        {
            _presets.with_mutable_ref(id, [&](Preset2& preset) {
                preset.values    = new_preset_values;
                _new_preset_name = "";
            });
        }
    }
    else
    {
        _current_preset_id = add({
            .name   = _new_preset_name,
            .values = new_preset_values,
        });
        _new_preset_name   = "";
    }
}

void PresetManager::imgui_adding_preset(const Settings& settings)
{
    const auto preset_id = find_preset_with_given_name(_new_preset_name);

    if (add_button(preset_id == PresetId{}, _new_preset_name))
    {
        save_preset(settings, preset_id);
    }

    ImGui::SameLine();
    ImGui::Text("as");
    ImGui::SameLine();

    imgui_name_input();
}

auto PresetManager::imgui(Settings& settings) -> bool
{
    _current_preset_id = find_preset_with_given_values(settings);

    bool settings_have_changed{false};

    settings_have_changed |= display_all_variables_widgets(settings);

    ImGui::Separator();

    const auto selected_id = dropdown("Presets", preset_name(_current_preset_id));

    settings_have_changed |= apply(selected_id, settings);

    if (!contains(_current_preset_id))
    {
        imgui_adding_preset(settings);
    }
    else
    {
        _presets.with_ref(_current_preset_id, [&](const Preset2& preset) {
            const auto current_name = preset.name;
            delete_button(_current_preset_id, current_name, *this);
            ImGui::SameLine();
            _rename_widget.imgui(_current_preset_id, current_name, *this);
        });
    }

    return settings_have_changed;
}

} // namespace Cool