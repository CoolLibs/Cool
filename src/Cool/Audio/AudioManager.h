#pragma once
#include <Cool/ImGui/IcoMoonCodepoints.h>
#include <Audio/lib/RtAudioWrapper/src/InputStream.hpp>
#include "Cool/Gpu/Texture.h"
#include "Cool/ImGui/ImGuiWindow.h"
#include "Cool/ImGui/icon_fmt.h"
#include "Cool/Log/MessageId.h"
#include "Cool/Time/Clock.h"
#include "Cool/Utils/Cached.h"
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

    /// Returns an array containing the raw audio samples, with values between -1 and 1. There is only one channel, so each sample corresponds to the next moment in time.
    [[nodiscard]] auto waveform() const -> std::vector<float> const&;
    /// Returns a 1D texture with all the waveform going from UV 0 to UV 1. Contains values between -1 and 1.
    [[nodiscard]] auto waveform_texture() const -> Texture const&;
    /// Returns an array where the values correspond to the amplitudes of frequencies evenly spaced between 0 and sample_rate / 2. Where sample_rate is the sample rate of the analysed audio.
    [[nodiscard]] auto spectrum() const -> std::vector<float> const&;
    /// Returns a 1D texture with all the spectrum going from UV 0 to UV 1. Contains values between 0 and +infinity.
    [[nodiscard]] auto spectrum_texture() const -> Texture const&;
    ///
    [[nodiscard]] auto volume() const -> float;

    void sync_with_clock(Cool::Clock const&, bool force_sync_time = false);
    /// Must be called every frame. Will do some internal bookkeeping, and call `on_audio_data_changed` iff the audio data has changed (e.g. music player is playing, or a microphone is beeing used).
    void update(std::function<void()> const& on_audio_data_changed);

    void set_current_input_mode(AudioInputMode);

    void open_imgui_window() { _window.open(); }
    void imgui_window();

    void set_force_mute(bool mute) { _file_input.set_force_mute(mute); }

private:
    void open_current_input_mode();

    [[nodiscard]] auto nb_frames_for_characteristics_computation(float window_size_in_seconds) const -> int64_t;

    void invalidate_caches();

    /// Returns the current audio input in use, as per `_current_input_mode`
    [[nodiscard]] auto current_input() -> internal::IAudioInput&;
    [[nodiscard]] auto current_input() const -> internal::IAudioInput const&;

private:
    internal::AudioInput_File   _file_input{};
    internal::AudioInput_Device _device_input{};
    AudioInputMode              _current_input_mode{};
    bool                        _audio_settings_have_changed{false}; // Used to signal that the app needs to rerender.

    float _spectrum_max_frequency_in_hz{2500.f}; // TODO(Audio) Serialize
    float _spectrum_max_amplitude{3.f};          // TODO(Audio) Serialize
    int   _spectrum_nb_bins{8};                  // TODO(Audio) Serialize

    mutable Cached<std::vector<float>> _current_waveform{};
    mutable Cached<std::vector<float>> _current_spectrum{};
    mutable Cached<float>              _current_volume{};
    mutable Cached<Texture>            _current_waveform_texture{};
    mutable Cached<Texture>            _current_spectrum_texture{};

    float _window_size_in_seconds_for_waveform{0.05f};
    float _window_size_in_seconds_for_spectrum{0.1f};
    float _window_size_in_seconds_for_volume{0.2f};

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
            cereal::make_nvp("Current input mode", _current_input_mode),
            cereal::make_nvp("Window size for waveform", _window_size_in_seconds_for_waveform),
            cereal::make_nvp("Window size for spectrum", _window_size_in_seconds_for_spectrum),
            cereal::make_nvp("Window size for volume", _window_size_in_seconds_for_volume)
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