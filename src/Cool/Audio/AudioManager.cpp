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

void AudioManager::play()
{
    RtAudioW::player().play();
}

void AudioManager::pause()
{
    RtAudioW::player().pause();
}

void AudioManager::set_time(float time_in_sec)
{
    RtAudioW::player().set_time(time_in_sec);
}

void AudioManager::sync_with_clock(Cool::Clock const& clock)
{
    if (clock.is_playing() /* && clock.delta_time() > 0.0001f */ /* TODO(Audio) should pause when the time is blocked on a number, eg while we are in input text on the timeline */)
    {
        play();
        static auto last_time = std::chrono::steady_clock::time_point{};
        auto const  now       = std::chrono::steady_clock::now();
        // if (now - last_time > 1s)
        {
            // set_time(clock.time());
            if (std::abs(clock.time() - time()) > 0.05f)
                set_time(clock.time());
            last_time = now;
        }
    }
    else
    {
        pause();
    }
}

auto AudioManager::time() const -> float
{
    return RtAudioW::player().get_time();
}

void AudioManager::update()
{
    RtAudioW::player().update_device_if_necessary();
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
        ImGui::SliderFloat("Volume", &RtAudioW::player().properties().volume, 0.f, 1.f); // TODO(Audio) GUI for all the properties
    });
}

void AudioManager::try_load_current_file()
{
    if (_audio_file_path.empty())
    {
        Cool::Log::ToUser::console().remove(_error_id);
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