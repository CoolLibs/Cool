#include "PresetManager.h"
#include <Cool/ImGui/ImGuiExtras.h>
#include <boxer/boxer.h>
#include "Cool/Variables/PresetManager.h"

// TODO(JF) In case of a change of Settings definition show a merge window that allows user to explicit the link between old and new names (for each old name that doesn't have a match in the new names, show a dropdown that allows to link it to one of the new names that don't correspond to any old names)

namespace Cool {

/// Returns the ID of the first Preset that matches the `predicate`, or a null ID if there was none.
static auto find_preset(
    reg::OrderedRegistry<Preset2> const&       presets,
    std::function<bool(const Preset2&)> const& predicate
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

auto PresetManager::find_preset_with_given_values(Settings_ConstRef values) const -> PresetId
{
    return find_preset(
        _presets, [&](const Preset2& preset) {
            return values.are_equal_to(preset.values);
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

static void set_default_value_to_current_value(Preset2& preset)
{
    for (auto& variable : preset.values)
    {
        std::visit([](auto&& variable) {
            variable.default_value = variable.value;
        },
                   variable);
    }
}

auto PresetManager::add(Preset2 preset, bool show_warning_messages) -> PresetId
{
    if (find_preset_with_given_values(preset.values) != PresetId{})
    {
        if (show_warning_messages)
        {
            boxer::show(
                "There is already a preset with the same values.",
                "Adding failed",
                boxer::Style::Warning,
                boxer::Buttons::OK
            );
        }
        return PresetId{};
    }
    else if (find_preset_with_given_name(preset.name) != PresetId{})
    {
        if (show_warning_messages)
        {
            boxer::show(
                "There is already a preset with the same name.",
                "Adding failed",
                boxer::Style::Warning,
                boxer::Buttons::OK
            );
        }
        return PresetId{};
    }
    else
    {
        set_default_value_to_current_value(preset);
        return _presets.create(preset);
    }
}

void PresetManager::edit(PresetId const& id, Settings const& new_values)
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
    const auto do_rename = [&]() {
        _presets.with_mutable_ref(id, [&](Preset2& preset) {
            preset.name = new_name;
        });
    };

    const auto preset_with_same_name = find_preset_with_given_name(new_name);

    if (preset_with_same_name == PresetId{}) // Make sure there isn't already a preset with the same name.
    {
        do_rename();
    }
    else if (preset_name(id) != new_name) // Don't warn if we are trying to rename a preset with its current name
    {
        const auto choice = boxer::show(
            fmt::format("There is already a preset called \"{}\", do you want to overwrite it?", new_name).c_str(),
            "Are you sure?",
            boxer::Style::Warning,
            boxer::Buttons::OKCancel
        );
        if (choice == boxer::Selection::OK)
        {
            remove(preset_with_same_name);
            do_rename();
        }
    }
}

auto PresetManager::preset_name(const PresetId& id) const -> std::optional<std::string>
{
    // Default return value
    std::optional<std::string> name{};

    // Replace nullopt with preset name if it is found in the registry
    _presets.with_ref(id, [&](const Preset2& preset) {
        name = preset.name;
    });

    return name;
}

auto PresetManager::apply(const PresetId& id, Settings_Ref settings) const -> bool
{
    return _presets.with_ref(id, [&](const Preset2& preset) {
        settings.assign_from(preset.values);
    });
}

void PresetManager::apply_first_preset_if_there_is_one(Settings_Ref settings) const
{
    std::shared_lock lock{_presets.mutex()};
    if (!is_empty())
    {
        settings.assign_from(_presets.begin()->second.values);
    }
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

static void delete_button(const PresetId& id, std::string_view name, PresetManager& preset_manager)
{
    if (ImGui::Button("Delete Preset"))
    {
        const auto choice = boxer::show(
            fmt::format("You are about to delete \"{}\", are you sure?", name).c_str(),
            "Are you sure?",
            boxer::Style::Warning,
            boxer::Buttons::OKCancel
        );
        if (choice == boxer::Selection::OK)
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

auto PresetManager::imgui_name_input() -> bool
{
    name_selector();
    ImGui::SameLine(0.f, 0.f);
    return ImGui::InputText("Name", &_new_preset_name, ImGuiInputTextFlags_EnterReturnsTrue);
}

static auto make_sure_the_user_wants_to_overwrite_the_preset(std::string_view new_preset_name) -> bool
{
    return boxer::show(
               fmt::format("You are about to overwrite \"{}\", are you sure?", new_preset_name).c_str(),
               "Are you sure?",
               boxer::Style::Warning,
               boxer::Buttons::OKCancel
           )
           == boxer::Selection::OK;
}

void PresetManager::save_preset(Settings_ConstRef new_preset_values, const PresetId& id)
{
    if (contains(id))
    {
        if (!make_sure_the_user_wants_to_overwrite_the_preset(_new_preset_name))
            return;

        _presets.with_mutable_ref(id, [&](Preset2& preset) {
            preset.values    = new_preset_values.as_settings();
            _new_preset_name = "";
        });
    }
    else
    {
        _current_preset_id = add({
            .name   = _new_preset_name,
            .values = new_preset_values.as_settings(),
        });
        _new_preset_name   = "";
    }

    _serializer.save(*this); // Save to file.
}

void PresetManager::imgui_adding_preset(Settings_ConstRef settings)
{
    const auto preset_id = find_preset_with_given_name(_new_preset_name);

    if (add_button(preset_id == PresetId{}, _new_preset_name))
    {
        save_preset(settings, preset_id);
    }

    ImGui::SameLine();
    ImGui::Text("as");
    ImGui::SameLine();

    if (imgui_name_input())
    {
        save_preset(settings, preset_id);
    }
}

auto PresetManager::imgui_presets(Settings_Ref settings) -> bool
{
    _current_preset_id = find_preset_with_given_values(settings);

    const auto current_preset_name = preset_name(_current_preset_id);

    const auto selected_id = dropdown("Presets", current_preset_name.value_or("Unsaved Settings..."));

    const auto settings_have_changed = apply(selected_id, settings);

    if (!current_preset_name)
    {
        imgui_adding_preset(settings);
    }
    else
    {
        delete_button(_current_preset_id, *current_preset_name, *this);
        ImGui::SameLine();
        _rename_widget.imgui(_current_preset_id, *current_preset_name, *this);
    }

    return settings_have_changed;
}

auto PresetManager::imgui(Settings_Ref settings) -> bool
{
    bool settings_have_changed{false};

    settings_have_changed |= settings.display_all_variables_widgets();

    ImGui::Separator();

    settings_have_changed |= imgui_presets(settings);

    return settings_have_changed;
}

} // namespace Cool