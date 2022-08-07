#include "Presets.h"
#include <Cool/ImGui/ImGuiExtras.h>
#include <optional>

// TODO(LD) In case of a rename show a merge window that allows user to explicit the link between old and new names (for each old name that doesn't have a match in the new names, show a dropsown that allows to link it to one of the new names that don't correspond to any old names)

namespace Cool {

auto PresetManager::current_name(
    const PresetId& id,
    const Settings& current_settings
) -> std::string
{
    std::string result = "Unsaved. . .";

    _presets.with_ref(id, [&](const Preset2& preset) {
        if (preset.values == current_settings)
        {
            result = preset.name;
        }
    });

    return result;
}

void PresetManager::apply_preset(PresetId id, Settings& preset_data)
{
    if (_presets.get(id) != std::nullopt)
    {
        preset_data = _presets.get(id)->values;
    }
}

auto PresetManager::dropdown(
    std::string_view dropdown_name,
    std::string_view current_name,
    Settings*        preset_data,
    ImGuiComboFlags_ flag = ImGuiComboFlags_None
) -> bool
{
    if (ImGui::BeginCombo(
            dropdown_name.data(),
            current_name.data(),
            flag
        ))
    {
        for (auto& [id, preset] : _presets)
        {
            bool is_selected = false;
            is_selected      = (_current_preset_id == id);
            if (_presets.get(_current_preset_id) != std::nullopt)
            {
                is_selected &= _presets.get(_current_preset_id)->name == current_name;
            }
            if (ImGui::Selectable(preset.name.c_str(), is_selected))
            {
                if (preset_data == nullptr && _presets.get(id) != std::nullopt)
                {
                    // current_name     = _presets.get(id)->name;
                    _new_preset_name = _presets.get(id)->name;
                }
                else
                {
                    _current_preset_id = id;
                    apply_preset(_current_preset_id, *preset_data);
                }
                ImGui::EndCombo();
                return true;
            }
        }
        ImGui::EndCombo();
    }
    return false;
}

auto PresetManager::name_selector() -> bool
{
    return dropdown(
        "Name",
        "",
        nullptr,
        ImGuiComboFlags_NoPreview
    );
}

static auto display_all_variables_widgets(Settings& preset_data) -> bool
{
    bool b = false;
    for (auto& variable : preset_data)
    {
        if (std::visit([](auto&& real_variable) { return imgui_widget(real_variable); }, variable))
        {
            b = true;
        }
    }
    return b;
}

static auto delete_button(PresetManager& preset_manager, const PresetId& id) -> bool
{
    if (preset_manager.contains(id))
    {
        if (ImGui::Button("Delete Preset"))
        {
            preset_manager.remove_preset(id);
            return true;
        }
    }
    return false;
}

auto PresetManager::add_button(Settings& preset_data) -> bool
{
    if (_new_preset_name == "")
    {
        Cool::ImGuiExtras::button_disabled("Create Preset", "Write a name before adding.");
        return false;
    }

    if (ImGui::Button("Create Preset"))
    {
        for (auto& [id, preset] : _presets)
        {
            if (_new_preset_name == preset.name)
            {
                if (preset_data == preset.values)
                {
                    return false;
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
                    _new_preset_name   = "";
                    _current_preset_id = id;
                    return true;
                }
                else
                {
                    return false;
                }
            }
        }
        const Preset2 new_preset = {
            .name   = _new_preset_name,
            .values = preset_data};
        _current_preset_id = add_preset(new_preset);
        _new_preset_name   = "";
        return true;
    }
    return false;
}

auto PresetManager::imgui(Settings& preset_data) -> bool
{
    display_all_variables_widgets(preset_data);

    _current_preset_id = get_id_by_given_values(preset_data);

    ImGui::Separator();

    delete_button(*this, _current_preset_id);
    if (!contains(_current_preset_id))
    {
        add_button(preset_data);
        ImGui::SameLine();
        ImGui::InputText("##465", &_new_preset_name);
        ImGui::SameLine();
        name_selector();
    }

    auto is_modified = false;
    is_modified |= dropdown(
        "Presets",
        current_name(_current_preset_id, preset_data),
        &preset_data
    );

    return is_modified;
}

} // namespace Cool