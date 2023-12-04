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
    auto compute_time() const -> float;

private:
    std::chrono::steady_clock::time_point _offset_with_std_time; // Allows us to set_time() as we wish
    std::chrono::steady_clock::time_point _std_time_when_paused;

    float _delta_time{0.f};
    float _prev_time{0.f};
    float _time{0.f};

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
        _time      = saved_time;
        _prev_time = saved_time;
    }
};

} // namespace Cool