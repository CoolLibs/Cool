#include "AudioInput_Device.h"
#include <Cool/ImGui/IcoMoonCodepoints.h>
#include "Cool/Log/ToUser.h"
#include "RtAudioWrapper/RtAudioWrapper.hpp"

namespace Cool::internal {

static auto input_stream() -> RtAudioW::InputStream&
{
    static RtAudioW::InputStream instance{
        [](RtAudioErrorType type, std::string const& error_message) {
            // RtAudioErrorType::
            // TODO(Audio) in case of device disconnect error, signal it directly to the audiomanager so that it can send a nice error to the user, with an error id
            Cool::Log::ToUser::warning("Audio", error_message);
        }};
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
    return has_device();
}

auto AudioInput_Device::has_device() const -> bool
{
    return true; // TODO(Audio)
}

void AudioInput_Device::update()
{
    // TODO(Audio)
}

void AudioInput_Device::start()
{
    open_device_stream();
}

void AudioInput_Device::stop()
{
    // close_device_stream(); // TODO(Audio)
}

auto AudioInput_Device::does_need_to_highlight_error() const -> bool
{
    return Cool::Log::ToUser::console().should_highlight(_error_id);
}

auto AudioInput_Device::imgui(bool needs_to_highlight_error) -> bool
{
    bool b = false;

    auto const ids = input_stream().device_ids();
    if (ImGui::BeginCombo("Input device", input_stream().current_device_name().c_str()))
    {
        for (auto const id : ids)
        {
            auto const info        = input_stream().device_info(id);
            bool const is_selected = info.name == input_stream().current_device_name();
            if (ImGui::Selectable(info.name.c_str(), is_selected))
            {
                input_stream().set_device(id);
                b = true;
            }

            if (is_selected) // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    b |= ImGui::DragFloat("Volume", &_volume, 20.f, 0.f, 1000000.f, "%.3f", ImGuiSliderFlags_Logarithmic);
    return b;
}

void AudioInput_Device::open_device_stream()
{
    // input_stream(). // TODO(Audio)
}

} // namespace Cool::internal