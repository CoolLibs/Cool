#pragma once

#include <Cool/Path/Path.h>
#include <Cool/Variables/PresetManager.h>

namespace Lab {

class TestPresets {
public:
    void imgui();

private:
    Cool::PresetManager _manager{Cool::Path::user_data() / "test-presets-manager.json"};

    Cool::Settings _current_settings{
        Cool::Variable<int>{{.name = "First", .value = 3}},
        Cool::Variable<float>{{.name = "Second", .value = 1.f}},
        Cool::Variable<float>{{.name = "Third", .value = 1.f}},
    };

    int _next_field_name = 0;
};

} // namespace Lab
