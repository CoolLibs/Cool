#include "TestPresets.h"

namespace Cool {

void TestPresets::imgui()
{
    _manager.imgui_presets(_current_settings);
    ImGui::NewLine();
    ImGui::Separator();
    ImGui::NewLine();
    if (ImGui::Button("Add a field in the settings"))
    {
        _current_settings.push_back(Cool::VariableData<int>{.name = std::to_string(_next_field_name++), .value = 5});
    }
    if (ImGui::Button("Remove a field in the settings"))
    {
        if (!_current_settings.empty())
        {
            _current_settings.pop_back();
        }
    }
    if (ImGui::Button("Add and edit a preset"))
    {
        const auto test_id = _manager.add({.name = "Test", .values = _current_settings});
        if (_manager.contains(test_id)) // Check if the adding was successful
        {
            _manager.edit(test_id, {
                                       Cool::VariableData<int>{.name = "First", .value = 0},
                                       Cool::VariableData<float>{.name = "Second", .value = 0.f},
                                       Cool::VariableData<float>{.name = "Third", .value = 0.f},
                                   });
        }
    }
}

} // namespace Cool
