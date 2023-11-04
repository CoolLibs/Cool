#include "AudioManager.h"
#include <Audio/lib/libnyquist/third_party/rtaudio/RtAudio.h>
#include <glpp-extended/src/TextureLayout.h>
#include <glpp/glpp.h>
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

// TODO(Audio) When waveform's window size is too big, then we can't fit all the samples in an opengl texture

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
    return _current_volume.get_value([&]() {
        auto frames = std::vector<float>{};
        current_input().for_each_audio_frame(nb_frames_for_characteristics_computation(_window_size_in_seconds_for_volume), [&](float frame) {
            frames.push_back(frame);
        });
        return Cool::compute_volume(frames);
    });
}

auto AudioManager::waveform() const -> std::vector<float> const&
{
    return _current_waveform.get_value([&]() {
        auto const N        = nb_frames_for_characteristics_computation(_window_size_in_seconds_for_waveform);
        auto       waveform = std::vector<float>{};
        current_input().for_each_audio_frame(N, [&](float sample) {
            waveform.push_back(sample);
        });
        return waveform;
    });
}

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

auto AudioManager::spectrum() const -> std::vector<float> const&
{
    return _current_spectrum.get_value([&]() {
        auto const N      = nb_frames_for_characteristics_computation(_window_size_in_seconds_for_spectrum);
        auto       myData = std::vector<std::complex<float>>{};
        myData.reserve(next_power_of_two(N));
        int i = 0;
        current_input().for_each_audio_frame(N, [&](float frame) {
            float t = i / (float)(N - 1);
            i++;
            float const window = 1.f - std::abs(2.f * t - 1.f); // Applying a window allows us to reduce "spectral leakage" https://digitalsoundandmusic.com/2-3-11-windowing-functions-to-eliminate-spectral-leakage/  // TODO(Audio-Philippe) Better windowing function? Or give the option to choose which windowing function to use? (I'm gonna create the widget anyways to test. If we do give the option, we need to specify next to each window what it is good at.)  We can pick from https://digitalsoundandmusic.com/2-3-11-windowing-functions-to-eliminate-spectral-leakage/
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
    });
}

static void set_texture_data(glpp::Texture1D& tex, std::vector<float> const& data)
{
    tex.upload_data(
        static_cast<GLsizei>(data.size()), data.data(),
        glpp::TextureLayout{
            .internal_format = glpp::InternalFormat::R16F,
            .channels        = glpp::Channels::R,
            .texel_data_type = glpp::TexelDataType::Float,
        }
    );
    glpp::set_wrap<glpp::TextureKind::Tex1D>(tex.id(), glpp::Wrap::ClampToBorder);
    glpp::set_magnification_filter<glpp::TextureKind::Tex1D>(tex.id(), glpp::Interpolation::Linear);
    glpp::set_minification_filter<glpp::TextureKind::Tex1D>(tex.id(), glpp::Interpolation::NearestNeighbour);
    GLfloat color[4] = {0.f, 0.f, 0.f, 0.f};                                  // NOLINT(*-avoid-c-arrays)
    GLDebug(glTexParameterfv(GL_TEXTURE_1D, GL_TEXTURE_BORDER_COLOR, color)); // TODO(JF) Wrap into glpp
}

auto AudioManager::waveform_texture() const -> glpp::Texture1D const&
{
    return _current_waveform_texture.get_value([&](glpp::Texture1D& tex) {
        set_texture_data(tex, waveform());
    });
}

auto AudioManager::spectrum_texture() const -> glpp::Texture1D const&
{
    return _current_spectrum_texture.get_value([&](glpp::Texture1D& tex) {
        set_texture_data(tex, spectrum());
    });
}

auto AudioManager::nb_frames_for_characteristics_computation(float window_size_in_seconds) const -> int64_t
{
    return static_cast<int64_t>(
        current_input().sample_rate() * window_size_in_seconds
    );
}

// TODO(Audio) Option to select how many bins we do (this is much more optimal to do binning on the cpu). This option should ideally be on each spectrum node, so that one can use the full spectrum while another is using only 8 bins.

void AudioManager::sync_with_clock(Cool::Clock const& clock, bool force_sync_time)
{
    if (force_sync_time
        || std::abs(clock.time() - RtAudioW::player().get_time()) > 0.5f // Syncing every frame sounds really bad, so we only sync when a gap has appeared.
    )
    {
        RtAudioW::player().set_time(clock.time()); // We sync even when the clock is paused because volume() needs the player to always be synced with the clock.
    }
    if (clock.is_playing() && !clock.is_being_forced_to_not_respect_realtime()) // Time is paused or frozen because the user is using the input text of the timeline to set the time value
        RtAudioW::player().play();
    else
        RtAudioW::player().pause();
}

void AudioManager::invalidate_caches()
{
    _current_waveform.invalidate_cache();
    _current_spectrum.invalidate_cache();
    _current_waveform_texture.invalidate_cache();
    _current_spectrum_texture.invalidate_cache();
    _current_volume.invalidate_cache();
}

void AudioManager::update(std::function<void()> const& on_audio_data_changed)
{
    if (current_input().is_playing() || _audio_settings_have_changed)
    {
        invalidate_caches();
        on_audio_data_changed();
        _audio_settings_have_changed = false;
    }
    current_input().update();
    _device_input.set_nb_of_retained_samples(
        std::max(
            std::max(
                nb_frames_for_characteristics_computation(_window_size_in_seconds_for_waveform),
                nb_frames_for_characteristics_computation(_window_size_in_seconds_for_spectrum)
            ),
            nb_frames_for_characteristics_computation(_window_size_in_seconds_for_volume)
        )
    );
}

static auto to_string(AudioInputMode mode) -> const char*
{
    switch (mode)
    {
    case AudioInputMode::File:
        return "File";
    case AudioInputMode::Device:
        return "Device";
    }
    return "";
}

void AudioManager::set_current_input_mode(AudioInputMode mode)
{
    if (mode == _current_input_mode)
        return;
    current_input().stop();
    _current_input_mode = mode;
    current_input().start();
    invalidate_caches();
    _audio_settings_have_changed = true;
}

void AudioManager::imgui_window()
{
    bool const needs_to_highlight_error = current_input().does_need_to_highlight_error();
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

        _audio_settings_have_changed |= current_input().imgui(needs_to_highlight_error);

        ImGui::NewLine();
        ImGui::SeparatorText("Volume");
        ImGui::ProgressBar(volume(), {0.f, 0.f});
        _audio_settings_have_changed |= ImGui::SliderFloat("Window size##Volume", &_window_size_in_seconds_for_volume, 0.f, 1.f, "%.3f seconds");

        ImGui::NewLine();
        ImGui::SeparatorText("Waveform");
        ImGui::PlotLines(
            "##Waveform",
            waveform().data(),
            static_cast<int>(waveform().size()),
            0, nullptr,
            -1.f, 1.f, // Values are between -1 and 1
            {0.f, 100.f}
        );
        _audio_settings_have_changed |= ImGui::SliderFloat("Window size##Waveform", &_window_size_in_seconds_for_waveform, 0.f, 1.f, "%.3f seconds");

        ImGui::NewLine();
        ImGui::SeparatorText("Spectrum");
        ImGui::PlotLines(
            "##Spectrum",
            spectrum().data(),
            static_cast<int>(spectrum().size()),
            0, nullptr,
            0.f, _spectrum_max_amplitude, // Values are between 0 and 1 // TODO(Audio) No they are not, cf code geass
            {0.f, 100.f}
        );
        _audio_settings_have_changed |= ImGui::SliderFloat("Window size##Spectrum", &_window_size_in_seconds_for_spectrum, 0.f, 0.5f, "%.3f seconds");
        _audio_settings_have_changed |= ImGui::SliderFloat("Max frequency displayed", &_spectrum_max_frequency_in_hz, 0.f, 22000.f, "%.0f Hertz");
        ImGui::DragFloat("Max amplitude displayed", &_spectrum_max_amplitude); // TODO(Audio) This slides way to fast, and can go below 0
    });
}

} // namespace Cool