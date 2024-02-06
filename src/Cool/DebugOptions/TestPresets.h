#pragma once

#include <Cool/Path/Path.h>
#include <Cool/Variables/PresetManager.h>

namespace Lab {

class TestPresets {
public:
    void imgui();

private:
    Cool::PresetManager _manager{{
        .user_defined_presets = Cool::Path::user_data() / "test-presets-manager.json",
        .default_presets      = "",
    }};

    Cool::Settings _current_settings{
        Cool::VariableData<int>{.name = "First", .value = 3},
        Cool::VariableData<float>{.name = "Second", .value = 1.f},
        Cool::VariableData<float>{.name = "Third", .value = 1.f},
    };

    int _next_field_name = 0;
};

} // namespace Lab
