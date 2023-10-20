#pragma once
#include <Cool/ImGui/IcoMoonCodepoints.h>
#include "Cool/ImGui/ImGuiWindow.h"
#include "Cool/ImGui/icon_fmt.h"
#include "Cool/Log/MessageId.h"
#include "Cool/Time/Clock.h"

namespace Cool {

class AudioManager {
public:
    void play();
    void pause();
    void set_time(float time_in_sec);
    auto time() const -> float;

    void sync_with_clock(Cool::Clock const&);

    void update();

    void open_imgui_window() { _window.open(); }
    void imgui_window();

private:
    void try_load_current_file();

private:
    std::filesystem::path _audio_file_path{};
    MessageId             _error_id{};
    ImGuiWindow           _window{icon_fmt("Audio", ICOMOON_MUSIC)};

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void save(Archive& archive) const
    {
        // TODO(Audio) Serialize the properties of the Player
        archive(
            cereal::make_nvp("Path", _audio_file_path)
        );
    }
    template<class Archive>
    void load(Archive& archive)
    {
        archive(
            _audio_file_path
        );
        try_load_current_file();
    }
};

} // namespace Cool