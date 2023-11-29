#include "AudioInput_Device.h"
#include <Cool/ImGui/IcoMoonCodepoints.h>
#include <Audio/src/InputStream.hpp>
#include <variant>
#include "Audio/Audio.hpp"
#include "Cool/ImGui/ImGuiExtras.h"
#include "Cool/Log/ToUser.h"

namespace Cool::internal {

static auto input_stream() -> Audio::InputStream&
{
    static Audio::InputStream instance{
        [](RtAudioErrorType type, std::string const& error_message) {
            if (type == RtAudioErrorType::RTAUDIO_DEVICE_DISCONNECT
                || type == RtAudioErrorType::RTAUDIO_INVALID_DEVICE
                || type == RtAudioErrorType::RTAUDIO_NO_DEVICES_FOUND)
            {
            }
            else
            {
                Cool::Log::ToUser::warning("Audio", error_message); // TODO(Audio) Don't show these warnings, they are useless
            }
        }
    };
    return instance;
}

void AudioInput_Device::set_nb_of_retained_samples(size_t samples_count)
{
    input_stream().set_nb_of_retained_samples(samples_count);
}

void AudioInput_Device::for_each_audio_frame(int64_t frames_count, std::function<void(float)> const& callback) const
{
    input_stream().for_each_sample(frames_count, [&](float const sample) { // We know that an input stream has only 1 channel, so 1 sample == 1 frame, no need to average across all the channels.
        callback(sample * _volume);
    });
}

auto AudioInput_Device::sample_rate() const -> float
{
    return static_cast<float>(input_stream().sample_rate());
}

auto AudioInput_Device::is_playing() const -> bool
{
    return input_stream().current_device_is_valid();
}

static auto get_device_name_impl(Audio::UseDefaultDevice) -> std::string
{
    return fmt::format("Use default device: {}", input_stream().device_info(input_stream().default_device_id()).name);
}
static auto get_device_name_impl(Audio::UseGivenDevice const& device) -> std::string
{
    return device.name;
}

static auto get_device_name(Audio::SelectedDevice const& selected_device) -> std::string
{
    return std::visit([](auto&& selected_device) { return get_device_name_impl(selected_device); }, selected_device);
}

void AudioInput_Device::update()
{
    input_stream().update();
    if (!input_stream().current_device_is_valid())
    {
        Cool::Log::ToUser::console().send(
            _error_id_device_invalid,
            Message{
                .category = "Audio",
                .message  = fmt::format("Invalid device \"{}\"", get_device_name(input_stream().current_device())),
                .severity = MessageSeverity::Error,
            }
        );
    }
    else
    {
        Cool::Log::ToUser::console().remove(_error_id_device_invalid);
    }
}

void AudioInput_Device::start()
{
    // No need to do anything on start, `input_stream().update()` will start the stream when necessary.
}

void AudioInput_Device::stop()
{
    input_stream().close();
}

auto AudioInput_Device::does_need_to_highlight_error() const -> bool
{
    return Cool::Log::ToUser::console().should_highlight(_error_id_device_invalid);
}

auto AudioInput_Device::imgui(bool needs_to_highlight_error) -> bool
{
    bool b = false;

    auto const ids = input_stream().device_ids();
    Cool::ImGuiExtras::bring_attention_if(
        needs_to_highlight_error,
        [&]() {
            auto const combo_text = get_device_name(input_stream().current_device());
            if (ImGui::BeginCombo("Input device", combo_text.c_str()))
            {
                bool const is_selected = std::holds_alternative<Audio::UseDefaultDevice>(input_stream().current_device());
                if (ImGui::Selectable(get_device_name_impl(Audio::UseDefaultDevice{}).c_str(), is_selected))
                    input_stream().use_default_device();
                if (is_selected) // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    ImGui::SetItemDefaultFocus();

                for (auto const id : ids)
                {
                    auto const info        = input_stream().device_info(id);
                    bool const is_selected = info.name == combo_text;
                    if (ImGui::Selectable(info.name.c_str(), is_selected))
                    {
                        input_stream().use_given_device(info);
                        b = true;
                    }

                    if (is_selected) // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
        }
    );
    b |= ImGui::DragFloat("Volume", &_volume, 20.f, 0.f, 1000000.f, "%.3f", ImGuiSliderFlags_Logarithmic);
    return b;
}

auto AudioInput_Device::get_input_device_selection() const -> Audio::SelectedDevice
{
    return input_stream().current_device();
}

void AudioInput_Device::set_input_device_selection(Audio::SelectedDevice device)
{
    input_stream().use_device(std::move(device));
}

} // namespace Cool::internal