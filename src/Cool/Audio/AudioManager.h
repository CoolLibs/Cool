#pragma once
#include <Cool/ImGui/IcoMoonCodepoints.h>
#include <Audio/lib/RtAudioWrapper/src/InputStream.hpp>
#include "Cool/ImGui/ImGuiWindow.h"
#include "Cool/ImGui/icon_fmt.h"
#include "Cool/Log/MessageId.h"
#include "Cool/Time/Clock.h"
#include "internal/AudioInput_Device.h"
#include "internal/AudioInput_File.h"
#include "internal/IAudioInput.h"

namespace cereal {

template<class Archive>
void serialize(Archive& archive, RtAudioW::PlayerProperties& properties)
{
    archive(
        cereal::make_nvp("Volume", properties.volume),
        cereal::make_nvp("Is muted", properties.is_muted),
        cereal::make_nvp("Does loop", properties.does_loop)
    );
}

} // namespace cereal

namespace Cool {

enum class AudioInputMode {
    File,
    Device,
};

class AudioManager {
public:
    AudioManager();

    [[nodiscard]] auto volume() const -> float;
    [[nodiscard]] auto spectrum() const -> std::vector<float> const&;

    void sync_with_clock(Cool::Clock const&);
    void update();

    void set_current_input_mode(AudioInputMode);

    void open_imgui_window() { _window.open(); }
    void imgui_window();

private:
    void open_current_input_mode();

    [[nodiscard]] auto compute_volume() const -> float;
    /// Returns an array where the values correspond to the amplitudes of frequencies evenly spaced between 0 and sample_rate / 2. Where sample_rate is the sample rate of the analysed audio.
    [[nodiscard]] auto compute_spectrum() const -> std::vector<float>;

    [[nodiscard]] auto nb_frames_for_characteristics_computation() const -> int64_t;

private:
    /// Returns the current audio input in use, as per `_current_input_mode`
    [[nodiscard]] auto current_input() -> internal::IAudioInput&;
    [[nodiscard]] auto current_input() const -> internal::IAudioInput const&;

private:
    internal::AudioInput_File   _file_input{};
    internal::AudioInput_Device _device_input{};
    AudioInputMode              _current_input_mode{};

    float _spectrum_max_frequency_in_hz{2500.f}; // TODO(Audio) Serialize
    float _spectrum_max_amplitude{3.f};          // TODO(Audio) Serialize
    int   _spectrum_nb_bins{8};                  // TODO(Audio) Serialize

    mutable float              _current_volume{};
    mutable bool               _current_volume_needs_recompute{true};
    mutable std::vector<float> _current_spectrum{};
    mutable bool               _current_spectrum_needs_recompute{true};

    float _average_duration_in_seconds{0.2f}; // TODO(Audio) Do we expose this ? If yes, then serialize it // TODO(Audio) The name should indicate that it is only used in volume calculation. Or do we use it for fft ? We try to find the nearest power of 2?

    ImGuiWindow _window{icon_fmt("Audio", ICOMOON_MUSIC)};

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void save(Archive& archive) const
    {
        archive(
            cereal::make_nvp("File input", _file_input),
            cereal::make_nvp("Device input", _device_input),
            cereal::make_nvp("Current input mode", _current_input_mode)
        );
    }
    template<class Archive>
    void load(Archive& archive)
    {
        archive(
            _file_input,
            _device_input,
            _current_input_mode
        );
        current_input().start();
    }
};

} // namespace Cool