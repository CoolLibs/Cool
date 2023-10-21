#pragma once

#include <chrono>
#include "Clock.h"

namespace Cool {

class Clock_Realtime : public Clock {
public:
    Clock_Realtime();
    ~Clock_Realtime() = default;

    auto delta_time() const -> float override;
    auto time() const -> float override;

    void set_time(float new_time) override;
    void update() override;

    void play() override;
    void pause() override;

private:
    inline std::chrono::steady_clock::time_point std_time() { return std::chrono::steady_clock::now(); }

private:
    std::chrono::steady_clock::time_point _initial_std_time;
    std::chrono::steady_clock::time_point _current_std_time;
    std::chrono::steady_clock::time_point _std_time_when_paused;
    float                                 _offset_with_std_time{0.f}; // Allows us to set_time() as we wish

    float _delta_time{0.f};
    float _prev_time{0.f};

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void save(Archive& archive) const
    {
        archive(
            cereal::make_nvp("Time", time()),
            cereal::make_nvp("Is playing", is_playing())
        );
    }
    template<class Archive>
    void load(Archive& archive)
    {
        float saved_time;       // NOLINT(cppcoreguidelines-init-variables)
        bool  saved_is_playing; // NOLINT(cppcoreguidelines-init-variables)
        archive(
            saved_time,
            saved_is_playing
        );
        set_time(saved_time);
        set_playing(saved_is_playing);
        _prev_time = saved_time;
    }
};

} // namespace Cool