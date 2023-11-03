#include "AudioInput_File.h"
#include <Cool/ImGui/IcoMoonCodepoints.h>
#include "Audio/Audio.hpp"
#include "Cool/ImGui/ImGuiExtras.h"
#include "Cool/Log/ToUser.h"
#include "Cool/NfdFileFilter/NfdFileFilter.h"

namespace Cool::internal {

void AudioInput_File::for_each_audio_frame(int64_t frames_count, std::function<void(float)> const& callback) const
{
    for (int i = 0; i < frames_count; ++i)
        callback(RtAudioW::player().sample_unaltered_volume(i - frames_count / 2 + RtAudioW::player().current_frame_index()));
}

auto AudioInput_File::sample_rate() const -> float
{
    return static_cast<float>(RtAudioW::player().audio_data().sample_rate);
}

auto AudioInput_File::is_playing() const -> bool
{
    return RtAudioW::player().is_playing();
}

void AudioInput_File::update()
{
    RtAudioW::player().properties() = _properties;
    RtAudioW::player().properties().is_muted |= _force_mute;
}

void AudioInput_File::start()
{
    try_load_current_file(); // TODO(Audio) don't reload the file if it was already loaded
}

void AudioInput_File::stop()
{
    RtAudioW::player().reset_audio_data(); // Stop the playing of the audio // TODO(Audio) instead, keep the file in memory but don't play it, so that it is faster to switch between input mode and we don't have to reload it over and over again
}

auto AudioInput_File::does_need_to_highlight_error() const -> bool
{
    return Cool::Log::ToUser::console().should_highlight(_error_id);
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

void AudioInput_File::imgui(bool needs_to_highlight_error)
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

void AudioInput_File::try_load_current_file()
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

} // namespace Cool::internal