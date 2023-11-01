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

namespace Cool {

AudioManager::AudioManager()
{
    current_input().start();
}

auto AudioManager::current_input() -> internal::IAudioInput&
{
    if (_current_input_mode == AudioInputMode::Device)
        return _device_input;
    return _file_input;
}
auto AudioManager::current_input() const -> internal::IAudioInput const&
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

auto AudioManager::nb_frames_for_characteristics_computation() const -> int64_t
{
    return static_cast<int64_t>(
        current_input().sample_rate() * _average_duration_in_seconds
    );
}

auto AudioManager::compute_volume() const -> float
{
    auto frames = std::vector<float>{};
    current_input().for_each_audio_frame(nb_frames_for_characteristics_computation(), [&](float frame) {
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
    current_input().for_each_audio_frame(N, [&](float frame) {
        float t = i / (float)(N - 1);
        i++;
        float const window = 1.f - std::abs(2.f * t - 1.f); // Applying a window allows us to reduce "spectral leakage" https://digitalsoundandmusic.com/2-3-11-windowing-functions-to-eliminate-spectral-leakage/  // TODO(Audio) Better windowing function? Or give the option to choose which windowing function to use? (I'm gonna create the widget anyways to test. If we do give the option, we need to specify next to each window what it is good at.)  We can pick from https://digitalsoundandmusic.com/2-3-11-windowing-functions-to-eliminate-spectral-leakage/
        myData.emplace_back(frame * window);
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
            _spectrum_max_frequency_in_hz / (static_cast<float>(current_input().sample_rate()) / static_cast<float>(data.size())) // The values in the `data` correspond to frequencies from 0 to sample_rate, evenly spaced.
        )
    ));
    return data;
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
    current_input().update();
    // TODO(Audio) if using device input, trigger rerender every frame
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
    // TODO(Audio) Trigger rerender when something changes
    if (mode == _current_input_mode)
        return;
    current_input().stop();
    _current_input_mode = mode;
    current_input().start();
}

void AudioManager::imgui_window()
{
    bool const needs_to_highlight_error = current_input().does_need_to_highlight_error();
    if (needs_to_highlight_error)
        _window.open();

    _window.show([&]() {
        // TODO(Audio) Trigger rerender when something changes
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
        current_input().imgui(needs_to_highlight_error);

        // ImGui::SliderFloat("Average duration", &_average_duration_in_seconds, 0.f, 1.f);
        // ImGui::Checkbox("Apply window", &_apply_window);

        ImGui::NewLine();
        ImGui::SeparatorText("Spectrum");
        ImGui::PlotLines(
            "Spectrum",
            spectrum().data(),
            static_cast<int>(spectrum().size()),
            0, nullptr,
            0.f, _spectrum_max_amplitude, // Values are between 0 and 1 // TODO(Audio) No they are not, cf code geass
            {0.f, 100.f}
        );
        ImGui::SliderFloat("Max frequency displayed", &_spectrum_max_frequency_in_hz, 0.f, 22000.f, "%.0f Hertz");
        ImGui::DragFloat("Max amplitude displayed", &_spectrum_max_amplitude); // TODO(Audio) This slides way to fast, and can go below 0
    });
}

} // namespace Cool