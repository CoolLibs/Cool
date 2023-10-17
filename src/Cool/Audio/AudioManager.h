#pragma once
#include "Cool/Log/MessageId.h"

namespace Cool {

class AudioManager {
public:
    void play();
    void pause();
    void seek_to(float time_in_sec);

    void imgui();

private:
    void try_load_current_file();

private:
    std::filesystem::path _audio_file_path{};
    MessageId             _error_id{};

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void save(Archive& archive) const
    {
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