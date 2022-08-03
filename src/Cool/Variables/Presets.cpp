#include "Presets.h"
#include <Cool/ImGui/ImGuiExtras.h>
#include <optional>

// TODO(LD) In case of a rename show a merge window that allows user to explicit the link between old and new names (for each old name that doesn't have a match in the new names, show a dropsown that allows to link it to one of the new names that don't correspond to any old names)

namespace Cool {

auto PresetManager::current_name(
    const std::optional<PresetId> id,
    PresetData&                   preset_data
) -> std::string
{
    if (id != std::nullopt)
    {
        if (_presets.get(*id) != std::nullopt)
        {
            if (_presets.get(*id)->values != preset_data)
            {
                return "Unsaved";
            }
            else
            {
                return _presets.get(*id)->name;
            }
        }
    }
    return "Unsaved";
}

void PresetManager::set_preset_data(PresetId id, PresetData& preset_data)
{
    if (_presets.get(*_current_preset_id) != std::nullopt)
    {
        preset_data = _presets.get(*_current_preset_id)->values;
    }
}

void PresetManager::dropdown(
    std::string_view           dropdown_name,
    std::string_view           current_name,
    std::optional<PresetData>& preset_data
)
{
    if (ImGui::BeginCombo(
            dropdown_name.data(),
            current_name.data(),
            0
        ))
    {
        for (auto& [id, preset] : _presets)
        {
            bool is_selected = false;
            if (_current_preset_id != std::nullopt)
            {
                is_selected = (*_current_preset_id == id);
                if (_presets.get(*_current_preset_id) != std::nullopt)
                {
                    is_selected &= _presets.get(*_current_preset_id)->name == current_name;
                }
            }
            if (ImGui::Selectable(preset.name.c_str(), is_selected))
            {
                if (preset_data == std::nullopt && _presets.get(id) != std::nullopt)
                {
                    current_name     = _presets.get(id)->name;
                    _name_selector   = _presets.get(id)->name;
                    _new_preset_name = _presets.get(id)->name;
                }
                else
                {
                    _current_preset_id = id;
                    set_preset_data(*_current_preset_id, *preset_data);
                }
            }
            if (is_selected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
}

void PresetManager::name_selector()
{
    std::optional<PresetData> null = std::nullopt;
    dropdown(
        "Name Selector",
        _name_selector,
        null
    );
}

auto PresetManager::display_all_variables_widgets(PresetData& preset_data)
{
    for (auto& variable : preset_data)
    {
        std::visit([](auto&& real_variable) { imgui_widget(real_variable); }, variable);
    }
}

auto PresetManager::delete_button(PresetData& preset_data)
{
    if (_current_preset_id != std::nullopt)
    {
        if (_presets.get(*_current_preset_id) != std::nullopt)
        {
            if (_presets.get(*_current_preset_id)->values == preset_data)
            {
                if (ImGui::Button("-"))
                {
                    auto id_to_delete = *_current_preset_id;
                    remove_preset(id_to_delete);
                }
            }
        }
    }
}

auto PresetManager::add_button(PresetData& preset_data)
{
    if (_new_preset_name == "")
    {
        Cool::ImGuiExtras::button_disabled("+", "Write a name before adding.");
        return;
    }

    if (ImGui::Button("+"))
    {
        for (auto& [id, preset] : _presets)
        {
            if (_new_preset_name == preset.name)
            {
                if (preset_data == preset.values)
                {
                    return;
                }
                const std::string warning_message = "Would you overwrite " + _new_preset_name + ", you will lose his previous values ?";

                auto sel = boxer::show(
                    warning_message.c_str(),
                    "Preset already exist.",
                    boxer::Style::Warning,
                    boxer::Buttons::YesNo
                );
                if (sel == boxer::Selection::Yes)
                {
                    preset.values      = preset_data;
                    _name_selector     = "none",
                    _new_preset_name   = "";
                    _current_preset_id = id;
                    return;
                }
                else
                {
                    return;
                }
            }
        }
        const Preset2 new_preset = {
            .name   = _new_preset_name,
            .values = preset_data};
        _current_preset_id = add_preset(new_preset);
        _new_preset_name   = "";
    }
}

auto PresetManager::imgui(PresetData& preset_data) -> bool
{
    auto optional_preset_data = std::make_optional(preset_data);
    dropdown(
        "Presets",
        current_name(_current_preset_id, preset_data),
        optional_preset_data
    );
    preset_data = *optional_preset_data; // dont't need to test if it's not nullopt because preset_data always exist.
    ImGui::Separator();

    name_selector();

    if (ImGui::InputText("Name", &_new_preset_name))
    {
        _name_selector = "none";
    }

    display_all_variables_widgets(preset_data);
    delete_button(preset_data);
    add_button(preset_data);

    return true;
}

} // namespace Cool