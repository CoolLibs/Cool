#pragma once
#include "Audio/Audio.hpp"
#include "IAudioInput.h"

namespace Cool::internal {

class AudioInput_File : public IAudioInput {
public:
    void               for_each_audio_frame(int64_t frames_count, std::function<void(float)> const& callback) const override;
    [[nodiscard]] auto sample_rate() const -> float override;
    [[nodiscard]] auto is_playing() const -> bool override;

    void update() override;
    void start() override;
    void stop() override;

    auto               imgui(bool needs_to_highlight_error) -> bool override;
    [[nodiscard]] auto does_need_to_highlight_error() const -> bool override;

    void set_force_mute(bool mute) { _force_mute = mute; }

private:
    void try_load_current_file();

private:
    std::filesystem::path   _path{};
    std::filesystem::path   _currently_loaded_path{};
    Audio::PlayerProperties _properties{};
    MessageId               _error_id{};

    bool _force_mute{false};

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Path", _path),
            cereal::make_nvp("Properties", _properties)
        );
    }
};

} // namespace Cool::internal