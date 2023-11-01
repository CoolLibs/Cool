#include "AudioInput_Device.h"
#include <Cool/ImGui/IcoMoonCodepoints.h>
#include "Cool/Log/ToUser.h"
#include "RtAudioWrapper/RtAudioWrapper.hpp"

namespace Cool::internal {

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

void AudioInput_Device::for_each_audio_frame(int64_t frames_count, std::function<void(float)> const& callback) const
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

auto AudioInput_Device::sample_rate() const -> float
{
    return static_cast<float>(input_stream().sample_rate());
}

void AudioInput_Device::update()
{
    // TODO(Audio)
    _audio_data = input;
    for (float& x : _audio_data)
        x *= _volume;
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

void AudioInput_Device::imgui(bool needs_to_highlight_error)
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

void AudioInput_Device::open_device_stream()
{
    // input_stream(). // TODO(Audio)
}

} // namespace Cool::internal