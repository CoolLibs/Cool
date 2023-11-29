#include "AudioFeature_Volume.h"

namespace Cool {

auto AudioFeature_Volume::volume() const -> float
{
    return _current_volume.get_value([&]() {
        if (Cool::DebugOptions::log_when_computing_audio_features())
            Cool::Log::ToUser::info("Audio", "Computing volume");

        auto frames = std::vector<float>{};
        current_input().for_each_audio_frame(nb_frames_for_characteristics_computation(_window_size_in_seconds_for_volume), [&](float frame) {
            frames.push_back(frame);
        });
        return Audio::compute_volume(frames);
    });
}

void AudioFeature_Volume::imgui()
{
}

} // namespace Cool