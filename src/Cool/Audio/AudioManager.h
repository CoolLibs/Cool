#pragma once
#include <Cool/ImGui/IcoMoonCodepoints.h>
#include "Cool/ImGui/ImGuiWindow.h"
#include "Cool/ImGui/icon_fmt.h"
#include "Cool/Log/MessageId.h"
#include "Cool/Time/Clock.h"
#include "RtAudioWrapper/RtAudioWrapper.hpp"

namespace cereal {

template<class Archive>
void serialize(Archive& archive, RtAudioW::PlayerProperties& properties)
{
    archive(
        cereal::make_nvp("Volume", properties.volume),
        cereal::make_nvp("Is muted", properties.is_muted),
        cereal::make_nvp("Does loop", properties.does_loop)
    );
}

} // namespace cereal

namespace Cool {

class AudioManager {
public:
    void               play();
    void               pause();
    void               set_time(float time_in_sec);
    void               sync_with_clock(Cool::Clock const&);
    [[nodiscard]] auto time() const -> float;

    void update();

    void open_imgui_window() { _window.open(); }
    void imgui_window();

private:
    void try_load_current_file();

private:
    std::filesystem::path      _audio_file_path{};
    RtAudioW::PlayerProperties _properties{};
    MessageId                  _error_id{};
    ImGuiWindow                _window{icon_fmt("Audio", ICOMOON_MUSIC)};

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void save(Archive& archive) const
    {
        archive(
            cereal::make_nvp("Path", _audio_file_path),
            cereal::make_nvp("Properties", _properties)
        );
    }
    template<class Archive>
    void load(Archive& archive)
    {
        archive(
            _audio_file_path,
            _properties
        );
        try_load_current_file();
    }
};

} // namespace Cool