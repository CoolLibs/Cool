#pragma once
#include "IAudioInput.h"

namespace Cool::internal {

class AudioInput_Device : public IAudioInput { // e.g. microphone
public:
    /// You MUST call this function at least once at the beginning to tell us the maximum numbers of samples you will query with `for_each_sample`.
    /// If that max number changes over time, you can call this function again to update it.
    void set_nb_of_retained_samples(size_t samples_count);

    void               for_each_audio_frame(int64_t frames_count, std::function<void(float)> const& callback) const override;
    [[nodiscard]] auto sample_rate() const -> float override;
    [[nodiscard]] auto is_playing() const -> bool override;

    void update() override;
    void start() override;
    void stop() override;

    auto               imgui(bool needs_to_highlight_error) -> bool override;
    [[nodiscard]] auto does_need_to_highlight_error() const -> bool override;

private:
    void               open_device_stream();
    [[nodiscard]] auto has_device() const -> bool;

private:
    float     _volume{30.f};
    MessageId _error_id_device_invalid{};

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            // cereal::make_nvp("Input Device ID", _input_device_id), // TODO(Audio) Serialize the SelectedDevice
            cereal::make_nvp("Volume", _volume)
        );
    }
};

} // namespace Cool::internal