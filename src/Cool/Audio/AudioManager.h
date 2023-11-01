#pragma once
#include <Cool/ImGui/IcoMoonCodepoints.h>
#include <Audio/lib/RtAudioWrapper/src/InputStream.hpp>
#include "Cool/ImGui/ImGuiWindow.h"
#include "Cool/ImGui/icon_fmt.h"
#include "Cool/Log/MessageId.h"
#include "Cool/Time/Clock.h"
#include "RtAudioWrapper/RtAudioWrapper.hpp"

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
    class IAudioInput {
    public:
        IAudioInput()                                               = default;
        IAudioInput(IAudioInput const&)                             = default;
        IAudioInput(IAudioInput&&)                                  = default;
        auto operator=(IAudioInput const&) noexcept -> IAudioInput& = default;
        auto operator=(IAudioInput&&) noexcept -> IAudioInput&      = default;
        virtual ~IAudioInput()                                      = default;

        /// Will call the `callback` once for each of the `frames_count` audio frames around the current moment in time.
        /// If there are several channels in a frame, the `callback` will only be called once for all of these channels, and will be given the average of the samples.
        virtual void for_each_audio_frame(int64_t frames_count, std::function<void(float)> const& callback) const = 0;
        /// Returns the number of frames contained in a second of the audio.
        [[nodiscard]] virtual auto sample_rate() const -> float = 0;

        virtual void               update()                                     = 0;
        virtual void               imgui(bool needs_to_highlight_error)         = 0;
        [[nodiscard]] virtual auto does_need_to_highlight_error() const -> bool = 0;
    };

    class AudioInput_File : public IAudioInput {
    public:
        void               for_each_audio_frame(int64_t frames_count, std::function<void(float)> const& callback) const override;
        [[nodiscard]] auto sample_rate() const -> float override;

        void               update() override;
        void               imgui(bool needs_to_highlight_error) override;
        [[nodiscard]] auto does_need_to_highlight_error() const -> bool override;

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

    class AudioInput_Device : public IAudioInput { // e.g. microphone
    public:
        void               for_each_audio_frame(int64_t frames_count, std::function<void(float)> const& callback) const override;
        [[nodiscard]] auto sample_rate() const -> float override;

        void               update() override;
        void               imgui(bool needs_to_highlight_error) override;
        [[nodiscard]] auto does_need_to_highlight_error() const -> bool override;

        void open_device_stream();

    private:
        std::vector<float> _audio_data{};
        float              _volume{30.f};
        unsigned int       _input_device_id{}; // TODO(Audio) Use this
        MessageId          _error_id{};        // TODO(Audio) error when the device is unavailable

    private:
        // Serialization
        friend class cereal::access;
        template<class Archive>
        void serialize(Archive& archive)
        {
            archive(
                cereal::make_nvp("Input Device ID", _input_device_id),
                cereal::make_nvp("Volume", _volume)
            );
        }
    };

    /// Returns the current audio input in use, as per `_current_input_mode`
    [[nodiscard]] auto audio_input() -> IAudioInput&;
    [[nodiscard]] auto audio_input() const -> IAudioInput const&;

private:
    AudioInput_File   _file_input{};
    AudioInput_Device _device_input{};
    AudioInputMode    _current_input_mode{};

    bool  _apply_window{true};
    float _spectrum_max_frequency_in_hz{5000}; // TODO(Audio) Serialize
    int   _spectrum_nb_bins{8};                // TODO(Audio) Serialize

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
        open_current_input_mode();
    }
};

} // namespace Cool