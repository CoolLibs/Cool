#include "AudioManager.h"
#include <Audio/lib/libnyquist/third_party/rtaudio/RtAudio.h>
#include <imgui.h>
#include <Audio/lib/RtAudioWrapper/src/InputStream.hpp>
#include <Audio/lib/RtAudioWrapper/src/Player.hpp>
#include <complex>
#include <exception>
#include "Audio/Audio.hpp"
#include "Cool/Audio/AudioManager.h"
#include "Cool/ImGui/ImGuiExtras.h"
#include "Cool/Log/Message.h"
#include "Cool/Log/MessageConsole.h"
#include "Cool/Log/ToUser.h"
#include "Cool/NfdFileFilter/NfdFileFilter.h"

namespace Cool {

std::vector<float> input{};

static auto input_stream() -> RtAudioW::InputStream&
{
    static RtAudioW::InputStream instance{
        [&](std::span<float const> buffer) {
            input.assign(buffer.begin(), buffer.end());
            // input.resize(buffer.size());
            // for(size_t i = 0; i < buffer.size(); ++i)
            // input[i] = buffer[i] * input_volume;
        },
        [](RtAudioErrorType type, std::string const& error_message) {
            // RtAudioErrorType::
            // TODO(Audio) in case of device disconnect error, signal it directly to the audiomanager so that it can send a nice error to the user, with an error id
            Cool::Log::ToUser::warning("Audio", error_message);
        }};
    return instance;
}

AudioManager::AudioManager()
{
    open_current_input_mode();
}

auto AudioManager::audio_input() -> IAudioInput&
{
    if (_current_input_mode == AudioInputMode::Device)
        return _device_input;
    return _file_input;
}
auto AudioManager::audio_input() const -> IAudioInput const&
{
    if (_current_input_mode == AudioInputMode::Device)
        return _device_input;
    return _file_input;
}

auto AudioManager::volume() const -> float
{
    if (_current_volume_needs_recompute)
    {
        _current_volume_needs_recompute = false;
        _current_volume                 = compute_volume();
    }
    return _current_volume;
}

auto AudioManager::spectrum() const -> std::vector<float> const&
{
    if (_current_spectrum_needs_recompute)
    {
        _current_spectrum_needs_recompute = false;
        _current_spectrum                 = compute_spectrum();
    }
    return _current_spectrum;
}

// auto AudioManager::AudioInput_File::compute_volume() const -> float
// {
//     return Cool::compute_volume(RtAudioW::player(), _average_duration_in_seconds);
// }

auto AudioManager::nb_frames_for_characteristics_computation() const -> int64_t
{
    return static_cast<int64_t>(
        audio_input().sample_rate() * _average_duration_in_seconds
    );
}

auto AudioManager::compute_volume() const -> float
{
    auto frames = std::vector<float>{};
    audio_input().for_each_audio_frame(nb_frames_for_characteristics_computation(), [&](float frame) {
        frames.push_back(frame);
    });
    return Cool::compute_volume(frames);
}

// TODO(Audio) Option to select how many bins we do (this is much more optimal to do binning on the cpu). This option should ideally be on each spectrum node, so that one can use the full spectrum while another is using only 8 bins.

template<std::integral T>
static auto next_power_of_two(T n) -> T
{
    if (n != 0 && !(n & (n - 1)))
        return n; // n is already a power of 2

    T power = 1;
    while (power < n)
    {
        power <<= 1;
    }

    return power;
}

/// Since the FFT requires a size that is a power of two, we add 0s at the end of the data.
/// https://mechanicalvibration.com/Zero_Padding_FFTs.html
static void zero_pad(std::vector<std::complex<float>>& data)
{
    data.resize(next_power_of_two(data.size()));
}

auto AudioManager::compute_spectrum() const -> std::vector<float>
{
    auto const N      = nb_frames_for_characteristics_computation(); // TODO(Audio). 8192 gives a nice spectrum, roughly matches the default _average_duration_in_seconds, but its FFT is a bit slow to compute). One solution would be to have a separate thread compute it, and then update the current one when its ready. We wouldn't wait on the computation o finish, just grabbed the latest computed value. Although in release it's really not that bad
    auto       myData = std::vector<std::complex<float>>{};
    myData.reserve(next_power_of_two(N));
    int i = 0;
    audio_input().for_each_audio_frame(N, [&](float frame) {
        float t = i / (float)(N - 1);
        i++;
        float const window = _apply_window
                                 ? 1.f - std::abs(2.f * t - 1.f) // TODO(Audio) Better windowing function?
                                 : 1.f;
        myData.emplace_back(frame) * window;
    });
    zero_pad(myData); // Make sure the size of myData is a power of 2.
    auto const fftData = dj::fft1d(myData, dj::fft_dir::DIR_FWD);
    auto       data    = std::vector<float>{};
    std::transform(fftData.begin(), fftData.end(), std::back_inserter(data), [](auto const x) {
        return std::abs(x);
    });
    data.resize(std::min(
        data.size() / 2, // The second half is a mirror of the first half, so we don't need it.
        static_cast<size_t>(
            _spectrum_max_frequency_in_hz / (static_cast<float>(audio_input().sample_rate()) / static_cast<float>(data.size())) // The values in the `data` correspond to frequencies from 0 to sample_rate, evenly spaced.
        )
    ));
    return data;
}

void AudioManager::AudioInput_File::for_each_audio_frame(int64_t frames_count, std::function<void(float)> const& callback) const
{
    for (int i = 0; i < frames_count; ++i)
        callback(RtAudioW::player().sample_unaltered_volume(i - frames_count / 2 + RtAudioW::player().current_frame_index()));
}

void AudioManager::AudioInput_Device::for_each_audio_frame(int64_t frames_count, std::function<void(float)> const& callback) const
{
    // TODO(Audio) store a long enough buffer, and the audio in callback appends to the end of that buffer, and deletes the beginning once the buffer is long enough

    for (int64_t i = 0; i < frames_count; ++i)
    {
        if (i < _audio_data.size())
            callback(_audio_data[i]);
        else
            callback(0.f);
    }
}

auto AudioManager::AudioInput_File::sample_rate() const -> float
{
    return static_cast<float>(RtAudioW::player().audio_data().sample_rate);
}

auto AudioManager::AudioInput_Device::sample_rate() const -> float
{
    return static_cast<float>(input_stream().sample_rate());
}

void AudioManager::sync_with_clock(Cool::Clock const& clock)
{
    if (std::abs(clock.time() - RtAudioW::player().get_time()) > 0.5f) // Syncing every frame sounds really bad, so we only sync when a gap has appeared.
        RtAudioW::player().set_time(clock.time());                     // We sync even when the clock is paused because volume() needs the player to always be synced with the clock.

    if (clock.is_playing() && !clock.is_being_forced_to_not_respect_realtime()) // Time is paused or frozen because the user is using the input text of the timeline to set the time value
        RtAudioW::player().play();
    else
        RtAudioW::player().pause();
}

void AudioManager::update()
{
    _current_spectrum_needs_recompute = true;
    _current_volume_needs_recompute   = true;
    audio_input().update();
    // TODO(Audio) if using device input, trigger rerender every frame
}
void AudioManager::AudioInput_File::update()
{
    RtAudioW::player().properties() = _properties;
}
void AudioManager::AudioInput_Device::update()
{
    // TODO(Audio)
    _audio_data = input;
    for (float& x : _audio_data)
        x *= _volume;
}

static auto to_string(AudioInputMode mode) -> const char*
{
    switch (mode)
    {
    case AudioInputMode::File:
        return "File (.mp3, etc.)";
    case AudioInputMode::Device:
        return "Device (Microphone, etc.)";
    }
    return "";
}

void AudioManager::set_current_input_mode(AudioInputMode mode)
{
    if (mode == _current_input_mode)
        return;
    _current_input_mode = mode;
    open_current_input_mode();
}

void AudioManager::open_current_input_mode()
{
    switch (_current_input_mode)
    {
    case AudioInputMode::File:
        _file_input.try_load_current_file(); // TODO(Audio) don't reload the file if it was already loaded
        break;
    case AudioInputMode::Device:
        RtAudioW::player().reset_audio_data(); // Stop the playing of the audio
        _device_input.open_device_stream();
        break;
    }
}

static void imgui_widgets(RtAudioW::PlayerProperties& props)
{
    ImGuiExtras::checkbox_button(ICOMOON_LOOP, &props.does_loop);
    ImGui::SetItemTooltip("%s", props.does_loop ? "The audio will loop." : "The audio will not loop. It will only play when the time is between 0 and the duration of the audio.");

    ImGui::SameLine();

    if (ImGuiExtras::button_with_text_icon(props.is_muted ? ICOMOON_VOLUME_MUTE2 : ICOMOON_VOLUME_MEDIUM))
        props.is_muted = !props.is_muted;
    ImGui::SetItemTooltip("%s", props.is_muted ? "Muted" : "Not muted");

    ImGui::SameLine();

    ImGuiExtras::disabled_if(props.is_muted, "The audio is muted.", [&]() {
        ImGui::SliderFloat("Volume", &props.volume, 0.f, 1.f);
    });
}

auto AudioManager::AudioInput_File::does_need_to_highlight_error() const -> bool
{
    return Cool::Log::ToUser::console().should_highlight(_error_id);
}
auto AudioManager::AudioInput_Device::does_need_to_highlight_error() const -> bool
{
    return Cool::Log::ToUser::console().should_highlight(_error_id);
}

void AudioManager::AudioInput_File::imgui(bool needs_to_highlight_error)
{
    Cool::ImGuiExtras::bring_attention_if(
        needs_to_highlight_error,
        [&]() {
            if (ImGuiExtras::file("Audio file", &_path, NfdFileFilter::Audio))
                try_load_current_file();
        }
    );
    imgui_widgets(_properties);
}
void AudioManager::AudioInput_Device::imgui(bool needs_to_highlight_error)
{
    auto const ids = input_stream().device_ids();
    if (ImGui::BeginCombo("Input device", input_stream().current_device_name().c_str()))
    {
        for (auto const id : ids)
        {
            auto const info        = input_stream().device_info(id);
            bool const is_selected = info.name == input_stream().current_device_name();
            if (ImGui::Selectable(info.name.c_str(), is_selected))
                input_stream().set_device(id);

            if (is_selected) // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    ImGui::DragFloat("Volume", &_volume, 20.f, 0.f, 1000000.f, "%.3f", ImGuiSliderFlags_Logarithmic);
}

void AudioManager::imgui_window()
{
    bool const needs_to_highlight_error = audio_input().does_need_to_highlight_error();
    if (needs_to_highlight_error)
        _window.open();

    _window.show([&]() {
        ImGui::SeparatorText("Input Selection");
        // Select the input mode
        if (ImGui::BeginCombo("Input Mode", to_string(_current_input_mode)))
        {
            int const mode = static_cast<int>(_current_input_mode);
            for (int i = 0; i < 2; ++i)
            {
                bool const is_selected = i == mode;
                if (ImGui::Selectable(to_string(static_cast<AudioInputMode>(i)), is_selected))
                {
                    set_current_input_mode(static_cast<AudioInputMode>(i));
                }

                if (is_selected) // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        // ImGui::SeparatorText(to_string(_current_input_mode));
        audio_input().imgui(needs_to_highlight_error);

        ImGui::SliderFloat("Average duration", &_average_duration_in_seconds, 0.f, 1.f);
        ImGui::SliderFloat("Spectrum max frequency (Hertz)", &_spectrum_max_frequency_in_hz, 0.f, 22000.f, "%.0f Hertz");
        ImGui::Checkbox("Apply window", &_apply_window);

        ImGui::NewLine();
        ImGui::SeparatorText("Spectrum");
        static float max_value{3.f}; // TODO(Audio)
        ImGui::PlotLines(
            "Spectrum",
            spectrum().data(),
            static_cast<int>(spectrum().size()),
            0, nullptr,
            0.f, max_value, // Values are between 0 and 1 // TODO(Audio) No they are not, cf code geass
            {0.f, 100.f}
        );
        // ImGui::InputInt("fft size", &N);
        ImGui::DragFloat("Max value", &max_value);
    });
}

void AudioManager::AudioInput_File::try_load_current_file()
{
    if (_path.empty())
    {
        Cool::Log::ToUser::console().remove(_error_id);
        RtAudioW::player().reset_audio_data();
        return;
    }

    try
    {
        load_audio_file(RtAudioW::player(), _path);
        Cool::Log::ToUser::console().remove(_error_id);
    }
    catch (std::exception& e)
    {
        Cool::Log::ToUser::console().send(
            _error_id,
            Message{
                .category = "Audio",
                .message  = e.what(),
                .severity = MessageSeverity::Error,
            }
        );
        RtAudioW::player().reset_audio_data();
    }
}

void AudioManager::AudioInput_Device::open_device_stream()
{
    // input_stream().
}

} // namespace Cool