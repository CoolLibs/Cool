#pragma once
#include <Cool/ImGui/IcoMoonCodepoints.h>
#include <Audio/Audio.hpp>
#include "Cool/ImGui/ImGuiWindow.h"
#include "Cool/ImGui/icon_fmt.h"
#include "Cool/Log/MessageId.h"
#include "Cool/Time/Clock.h"
#include "Cool/Utils/Cached.h"
#include "glpp-extended/src/Texture1D.h"
#include "internal/AudioInput_Device.h"
#include "internal/AudioInput_File.h"
#include "internal/IAudioInput.h"

// TODO(Audio) Chaque AudioFeature possède se propre Source qui dit d'où il veut prendre l'audio (quel fichier, ou input device)
// et ses caractéristiques (durée de la fenêtre etc.). L'imgui n'affiche pas la feature (car relou d'accéder à l'audio manager pendant imgui, +pas nécessaire c'est on a de bons visualiseurs par défaut)
// Pour savoir si le cache est invalide, l'audio library stocke un id, qui change à chaque fois que l'audio change, et la feature compare l'id courant à l'id de quand elle s'est calculée la dernière fois
// Et pour savoir combien de samples garder pour les input devices, l'AudioLibrary stocke la liste du max demandé, sur quelques frames, pour toujours s'adapter au nombre courant de samples demandé
// TODO(Audio) et ça serait bien de pouvoir référencer une variable "AudioSource", pour que si on a 3 nodes audio qui doivent toujours utiliser la même source, on n'ait pas besoin de changer ça en plusieurs endroits
// Ou alors, il y a une MainAudioSource sur le Manager, et on peut en créer d'autres au besoin. Et les AudioFeature ont un dropdown pour savoir quelle source utiliser, par défaut ça sera MainAudioSource.

namespace cereal {

template<class Archive>
void serialize(Archive& archive, Audio::PlayerProperties& properties)
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
    [[nodiscard]] auto waveform_texture() const -> glpp::Texture1D const&;
    /// Returns an array where the values correspond to the amplitudes of frequencies evenly spaced between 0 and sample_rate / 2. Where sample_rate is the sample rate of the analysed audio.
    [[nodiscard]] auto spectrum() const -> Audio::Spectrum const&;
    /// Returns a 1D texture with all the spectrum going from UV 0 to UV 1. Contains values between 0 and +infinity.
    [[nodiscard]] auto spectrum_texture() const -> glpp::Texture1D const&;

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
    float _spectrum_scale_height{1.f};           // TODO(Audio) Serialize
    int   _spectrum_nb_bins{8};                  // TODO(Audio) Serialize

    mutable Cached<std::vector<float>> _current_waveform{};
    mutable Cached<glpp::Texture1D>    _current_waveform_texture{};
    mutable Cached<Audio::Spectrum>    _current_spectrum{};
    mutable Cached<glpp::Texture1D>    _current_spectrum_texture{};

    float _window_size_in_seconds_for_waveform{0.05f};
    float _window_size_in_seconds_for_spectrum{0.1f};

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