#pragma once

#include <chrono>
#include "Clock.h"

namespace Cool {

class Clock_Realtime : public Clock {
public:
    Clock_Realtime();
    ~Clock_Realtime() = default;

    float delta_time() const override;
    float time() const override;

    void set_time(float new_time) override;
    void update() override;

    void play() override;
    void pause() override;

private:
    inline std::chrono::steady_clock::time_point std_time() { return std::chrono::steady_clock::now(); }

private:
    std::chrono::steady_clock::time_point _initial_time;
    std::chrono::steady_clock::time_point _last_time;
    std::chrono::steady_clock::time_point _current_time;
    std::chrono::steady_clock::time_point _time_when_paused;
    float                                 _offset_with_std_time = 0.f; // Allows us to set_time as we wish

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void save(Archive& archive) const
    {
        archive(
            cereal::make_nvp("Time", time())
        );
    }
    template<class Archive>
    void load(Archive& archive)
    {
        float saved_time; // NOLINT(cppcoreguidelines-init-variables)
        archive(
            saved_time
        );
        set_time(saved_time);
    }
};

} // namespace Cool