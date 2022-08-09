#include "Presets.h"
#include <Cool/ImGui/ImGuiExtras.h>
#include <optional>

// TODO(LD) In case of a rename show a merge window that allows user to explicit the link between old and new names (for each old name that doesn't have a match in the new names, show a dropsown that allows to link it to one of the new names that don't correspond to any old names)

namespace Cool {

auto PresetManager::find_preset_with_given_values(const Settings& values) const -> PresetId
{
    for (const auto& [id, preset] : _presets)
    {
        if (preset.values == values)
        {
            return id;
        }
    }
    return PresetId{};
}

// TODO(LD) Need to be test ?
// TODO(LD) Add warning message.
void PresetManager::edit_preset_values(const PresetId& current_id, const Settings& settings)
{
    for (auto& [id, preset] : _presets)
    {
        if (current_id == id && find_preset_with_given_values(settings) == PresetId{})
        {
            preset.values = settings;
        }
    }
}

// TODO(LD) Add warning message.
void PresetManager::rename_preset(const PresetId& current_id, std::string_view name)
{
    for (auto& [id, preset] : _presets)
    {
        if (current_id == id)
        {
            if (find_preset_with_same_name(name) == nullptr)
            {
                preset.name = name;
            }
            else
            {
                boxer::show(
                    "You can't rename as an already existing preset's name !",
                    "Name already exists.",
                    boxer::Style::Warning,
                    boxer::Buttons::OK
                );
            }
        }
    }
}

auto PresetManager::preset_name(const PresetId& preset_id) const -> std::string
{
    // Default name
    std::string name = "Unsaved Settings...";

    // Replace default name with preset name if it is found in the registry
    // TODO(LD) _presets.with_ref(preset_id, [&](const Preset2& preset) {
    //     name = preset.name;
    // });
    auto preset = _presets.get(preset_id);
    if (preset)
    {
        name = preset->name;
    }

    return name;
}

auto PresetManager::apply_preset(const PresetId& id, Settings& settings) const -> bool
{
    const auto preset = _presets.get(id);
    if (preset)
    {
        settings = preset->values;
        return true; // NOLINT
    }
    else
    {
        return false;
    }
}

auto PresetManager::dropdown(
    std::string_view dropdown_name,
    std::string_view current_name,
    ImGuiComboFlags  combo_flags
) -> PresetId
{
    auto current_id = PresetId{};

    if (ImGui::BeginCombo(
            dropdown_name.data(),
            current_name.data(),
            combo_flags
        ))
    {
        for (const auto& [id, preset] : _presets)
        {
            ImGui::PushID(&preset);
            const bool is_selected = _current_preset_id == id;
            if (ImGui::Selectable(preset.name.c_str(), is_selected))
            {
                current_id = id;
            }
            ImGui::PopID();
        }
        ImGui::EndCombo();
    }
    return current_id;
}

void PresetManager::name_selector()
{
    // Draw just the arrow of the dropdown
    const auto id = dropdown("Name", "", ImGuiComboFlags_NoPreview);

    // Update _new_preset_name if a preset was selected in the dropdown
    _presets.with_ref(id, [&](const Preset2& preset) {
        _new_preset_name = preset.name;
    });
}

static auto display_all_variables_widgets(Settings& settings) -> bool
{
    bool was_used = false;
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
        const auto sel = boxer::show(
            fmt::format("Would you delete {}, you will lose it definitively ?", name).c_str(),
            "Preset already exists.",
            boxer::Style::Warning,
            boxer::Buttons::YesNo
        );
        if (sel == boxer::Selection::Yes)
        {
            preset_manager.remove_preset(id);
        }
    }
}

void PresetManager::RenameWidget::rename_button(PresetId& id, std::string_view current_name, PresetManager& preset_manager)
{
    const bool is_open_last_frame = _is_popup_open;
    _is_popup_open                = ImGuiExtras::begin_popup_context_menu_from_button("Rename");
    if (_is_popup_open)
    {
        if (!is_open_last_frame)
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
    if (is_open_last_frame && !_is_popup_open) // Was just closed
    {
        preset_manager.rename_preset(id, _new_name);
    }
}

auto PresetManager::find_preset_with_same_name(std::string_view name) -> Preset2*
{
    for (auto& [_, current_preset] : _presets)
    {
        if (name == current_preset.name)
        {
            return &current_preset;
        }
    }
    return nullptr;
}

auto add_button(Preset2* preset, std::string_view name) -> bool
{
    auto is_clicked = false;
    Cool::ImGuiExtras::maybe_disabled(name == "", "Write a name before adding.", [&]() {
        if (preset == nullptr)
        {
            is_clicked = ImGui::Button("Save Preset");
        }
        else
        {
            is_clicked = Cool::ImGuiExtras::colored_button("Overwrite", 0.5);
        }
    });
    return is_clicked;
}

void PresetManager::input_name()
{
    ImGui::SameLine(0.f, 0.f);
    ImGui::InputText("##465", &_new_preset_name);
    ImGui::SameLine(0.f, 0.f);
    name_selector();
}

void PresetManager::overwrite_preset(Preset2* preset, const Settings& settings)
{
    const auto sel = boxer::show(
        fmt::format("Would you overwrite {}, you will lose his previous values ?", _new_preset_name).c_str(),
        "Preset already exists.",
        boxer::Style::Warning,
        boxer::Buttons::YesNo
    );
    if (sel == boxer::Selection::Yes)
    {
        preset->values   = settings;
        _new_preset_name = "";
    }
}

void PresetManager::save_preset(Preset2* preset, const Settings& settings)
{
    if (preset != nullptr)
    {
        overwrite_preset(preset, settings);
    }
    else
    {
        _current_preset_id = add_preset({
            .name   = _new_preset_name,
            .values = settings,
        });
        _new_preset_name   = "";
    }
}

void PresetManager::adding_preset_interface(const Settings& settings)
{
    Preset2* preset = find_preset_with_same_name(_new_preset_name);

    const auto button_clicked = add_button(preset, _new_preset_name);

    ImGui::SameLine();
    ImGui::Text("as");

    input_name();

    if (button_clicked)
    {
        save_preset(preset, settings);
    }
}

auto PresetManager::imgui(Settings& settings) -> bool
{
    bool settings_have_changed = false;

    settings_have_changed |= display_all_variables_widgets(settings);

    _current_preset_id = find_preset_with_given_values(settings);

    ImGui::Separator();

    const auto id = dropdown("Presets", preset_name(_current_preset_id));

    settings_have_changed |= apply_preset(id, settings);

    if (!contains(_current_preset_id))
    {
        adding_preset_interface(settings);
    }
    else
    {
        if (_presets.get(_current_preset_id))
        {
            const auto current_name = _presets.get(_current_preset_id)->name;
            delete_button(_current_preset_id, current_name, *this);
            ImGui::SameLine();
            _rename_widget.rename_button(_current_preset_id, current_name, *this);
        }
    }

    return settings_have_changed;
}

} // namespace Cool