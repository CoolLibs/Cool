#include "AudioManager.h"
#include <imgui.h>
#include <exception>
#include "Audio/Audio.hpp"
#include "Cool/ImGui/ImGuiExtras.h"
#include "Cool/Log/Message.h"
#include "Cool/Log/MessageConsole.h"
#include "Cool/Log/ToUser.h"
#include "Cool/NfdFileFilter/NfdFileFilter.h"

namespace Cool {

static auto audio_player() -> RtAudioW::Player&
{
    static auto instance = RtAudioW::Player{};
    return instance;
}

void AudioManager::play()
{
    audio_player().play();
}

void AudioManager::pause()
{
    audio_player().pause();
}

void AudioManager::seek_to(float time_in_sec)
{
    audio_player().seek(time_in_sec);
}

void AudioManager::imgui()
{
    if (ImGuiExtras::file("Audio file", &_audio_file_path, NfdFileFilter::Audio))
        try_load_current_file();
    auto volume = audio_player().get_volume();
    if (ImGui::SliderFloat("Volume", &volume, 0.f, 1.f))
        audio_player().set_volume(volume);
}

void AudioManager::try_load_current_file()
{
    try
    {
        load_audio_file(audio_player(), _audio_file_path);
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
        audio_player().close();
    }
}

} // namespace Cool