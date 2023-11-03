#pragma once
#include "IAudioInput.h"
#include "RtAudioWrapper/RtAudioWrapper.hpp"

namespace Cool::internal {

class AudioInput_File : public IAudioInput {
public:
    void               for_each_audio_frame(int64_t frames_count, std::function<void(float)> const& callback) const override;
    [[nodiscard]] auto sample_rate() const -> float override;
    [[nodiscard]] auto is_playing() const -> bool override;

    void update() override;
    void start() override;
    void stop() override;

    void               imgui(bool needs_to_highlight_error) override;
    [[nodiscard]] auto does_need_to_highlight_error() const -> bool override;

private:
    void try_load_current_file();

private:
    std::filesystem::path      _path{};
    RtAudioW::PlayerProperties _properties{};
    MessageId                  _error_id{};

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