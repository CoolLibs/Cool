#include "AudioManager.h"
#include <imgui.h>
#include <exception>
#include "Audio/Audio.hpp"
#include "Cool/Audio/AudioManager.h"
#include "Cool/ImGui/ImGuiExtras.h"
#include "Cool/Log/Message.h"
#include "Cool/Log/MessageConsole.h"
#include "Cool/Log/ToUser.h"
#include "Cool/NfdFileFilter/NfdFileFilter.h"

namespace Cool {

void AudioManager::sync_with_clock(Cool::Clock const& clock)
{
    if (clock.is_playing() && !clock.is_being_forced_to_not_respect_realtime()) // Time is paused or frozen because the user is using the input text of the timeline to set the time value
    {
        RtAudioW::player().play();
        if (std::abs(clock.time() - RtAudioW::player().get_time()) > 0.05f) // Syncing every frame sounds really bad, so we only sync when a gap has appeared.
            RtAudioW::player().set_time(clock.time());
    }
    else
    {
        RtAudioW::player().pause();
    }
}

void AudioManager::update()
{
    RtAudioW::player().properties() = _properties;
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

    ImGuiExtras::disabled_if(props.is_muted, "The volume is muted.", [&]() {
        ImGui::SliderFloat("Volume", &props.volume, 0.f, 1.f);
    });
}

void AudioManager::imgui_window()
{
    bool const needs_to_highlight_error = Cool::Log::ToUser::console().should_highlight(_error_id);
    if (needs_to_highlight_error)
        _window.open();

    _window.show([&]() {
        Cool::ImGuiExtras::bring_attention_if(
            needs_to_highlight_error,
            [&]() {
                if (ImGuiExtras::file("Audio file", &_audio_file_path, NfdFileFilter::Audio))
                    try_load_current_file();
            }
        );
        imgui_widgets(_properties);
    });
}

void AudioManager::try_load_current_file()
{
    if (_audio_file_path.empty())
    {
        Cool::Log::ToUser::console().remove(_error_id);
        RtAudioW::player().reset_audio_data();
        return;
    }

    try
    {
        load_audio_file(RtAudioW::player(), _audio_file_path);
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

} // namespace Cool