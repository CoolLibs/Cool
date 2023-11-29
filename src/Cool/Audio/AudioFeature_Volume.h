#pragma once
#include "Cool/Audio/AudioManager.h"
#include "Cool/Utils/Cached.h"

namespace Cool {

class AudioManager;

class AudioFeature_Volume {
public:
    [[nodiscard]] auto get_value(AudioManager const&) const -> float;

    void imgui();

private:
    mutable Cached<float> _current_volume{};

    float _window_size_in_seconds{0.2f};
};

} // namespace Cool